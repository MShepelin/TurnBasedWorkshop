// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveAbility.h"
#include "Workshop/Builders/BuildAbility.h"

FString AInteractiveAbility::GatherInformation() const
{
  FString AbilityInformation = Super::GatherInformation();

  for (int32 ArrayIndex = 0; ArrayIndex < CTsToAffect.Num() - 1; ArrayIndex++)
  {
    if (ArrayIndex)
    {
      AbilityInformation += ", ";
    }
    AbilityInformation += MainManager->GetCTName(CTsToAffect[ArrayIndex]);
  }

  if (CTsToAffect.Num() > 1)
  {
    AbilityInformation += " or ";
  }

  if (CTsToAffect.Num())
  {
    AbilityInformation += MainManager->GetCTName(CTsToAffect[CTsToAffect.Num() - 1]) + " ";
  }

  /*
  if (TargetType != EInteractiveType::Any)
  {
    AbilityInformation += UEnum::GetValueAsString(TargetType).RightChop(LengthOfInteractiveTypeName) + " ";
  }
  //++++ add mask type support
  */

  for (UEffectData* EffectObject : UsedEffects)
  {
    if (!EffectObject)
    {
      UE_LOG(LogTemp, Error, TEXT("Unknown error!"));
      continue;
    }
    
    AbilityInformation += EffectObject->GatherInformation(true, MainManager);
  }

  return AbilityInformation;
}

void AInteractiveAbility::ShowInfluences() const
{
  Super::ShowInfluences();
}

AInteractiveAbility::AInteractiveAbility()
{
  InteractiveType = static_cast<int32>(EInteractiveType::Ability);

  AbilityPresentation = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("AbilitySprite"));
  AbilityPresentation->SetupAttachment(RootComponent);

  AbilityPresentation->SetRelativeLocation(FVector(0, -1, 0)); // y-order
}

void AInteractiveAbility::SetCharacterOwner(AInteractiveCharacter* NewCharacterOwner)
{
  if (CharacterOwner)
  {
    UE_LOG(LogTemp, Error, TEXT("Owner can be initialised only once!"));
    return;
  }

  CharacterOwner = NewCharacterOwner;
}

void AInteractiveAbility::CustomEffect_Implementation(AInteractiveObject* TargetObject)
{
  UBuildAbility::AddAllEffectsToObject(TargetObject, this);
}

void AInteractiveAbility::ResolveAbility()
{
  check(CharacterOwner != nullptr)

  CharacterOwner->PlayAnimation(AbilityAnimationId);
  
  //++++ apply movememt

  for (AInteractiveObject* DependentObject : InfluencesArray)
  {
    CustomEffect(DependentObject);
  }

  ClearInflunces();
}

void AInteractiveAbility::PostInitProperties()
{
  Super::PostInitProperties();

  if (IconScene)
  {
    AbilityPresentation->SetFlipbook(IconScene);
  }
}

void AInteractiveAbility::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);
}

void AInteractiveAbility::BeginPlay()
{
  Super::BeginPlay();
}

void AInteractiveAbility::PickedAsCentral()
{
  Super::PickedAsCentral();

  MainManager->FindObjectsByCTs(CTsToAffect, 1);
  TArray<AInteractiveObject*>& FoundObjects = MainManager->FoundObjects;
  AInteractiveObject* SelfPointer = Cast<AInteractiveObject>(this);

  int32 AtLeastOneMask = TargetTypeMask & InteractiveTypeSeparator;
  int32 NecessaryMask = TargetTypeMask & ~InteractiveTypeSeparator;

  for (size_t ObjectIndex = 0; ObjectIndex < FoundObjects.Num(); ObjectIndex++)
  {
    AInteractiveObject* FoundObject = FoundObjects[ObjectIndex];
    int32 AtLeastOneMaskTarget = (FoundObject->GetInteractiveType() & TargetTypeMask) & InteractiveTypeSeparator;
    int32 NecessaryMaskTarget = (FoundObject->GetInteractiveType() & TargetTypeMask) & ~InteractiveTypeSeparator;

    if ((AtLeastOneMaskTarget & AtLeastOneMask) && ((NecessaryMaskTarget & NecessaryMask) == NecessaryMask) && SelfPointer != FoundObject)
    {
      continue;
    }

    FoundObjects.Swap(ObjectIndex, FoundObjects.Num() - 1);
    FoundObjects.Pop();
  }

#if WITH_EDITOR
  UE_LOG(LogTemp, Warning, TEXT("%d targets found"), FoundObjects.Num());

  // Show what objects were found
  for (AInteractiveObject* FoundObject : MainManager->FoundObjects)
  {
    DrawDebugLine(GetWorld(), GetActorLocation(), FoundObject->GetActorLocation(),
      DebugColor, false, DebugTime);
  }
#endif

  ShowIconsDependingOnInfluence();
}

void AInteractiveAbility::UnpickedAsCentral()
{
  Super::UnpickedAsCentral();

  HideDisplayedIcons();
}

void AInteractiveAbility::PickedAsTarget()
{
  Super::PickedAsTarget();
}

void AInteractiveAbility::UnpickedAsTarget()
{
  Super::UnpickedAsTarget();
}

int32 AInteractiveAbility::GetTargetTypeMask() const
{
  return TargetTypeMask;
}

void AInteractiveAbility::SetTurn(ETurnPhase TurnPhase)
{
  for (size_t EffectIndex = 0; EffectIndex < AccumulatedEffects.Num(); EffectIndex++)
  {
    UEffectData* ChosenEffect = AccumulatedEffects[EffectIndex];

    if (ChosenEffect->TurnPhaseToResolve != TurnPhase)
    {
      continue;
    }

    ChosenEffect->ResolveOn(this);
    ChosenEffect->DecreaseDuration();

    // Remove effect if it is no longer present
    if (!ChosenEffect->Duration)
    {
      RemoveEffectByIndex(EffectIndex);
    }
  }

  for (size_t EffectIndex = UsedEffects.Num() - 1; EffectIndex >= 0; EffectIndex--)
  {
    UEffectData* ChosenEffect = UsedEffects[EffectIndex];

    if (ChosenEffect->TurnPhaseToResolve != TurnPhase)
    {
      continue;
    }

    if (!ChosenEffect->bIsBonusEffect)
    {
      break;
    }

    ChosenEffect->DecreaseDuration();

    // Remove effect if it is no longer present
    if (!ChosenEffect->Duration)
    {
      UsedEffects.Swap(EffectIndex, UsedEffects.Num() - 1);
      UsedEffects.Pop();
    }
  }
}

void AInteractiveAbility::CenterInOwner()
{
  check(CharacterOwner != nullptr);
  CharacterOwner->SetCentralAbility(this);
  CharacterOwner->SetCentralAbilityVisibility(false);
}

UTexture2D* AInteractiveAbility::GetIconUI() const
{
  return IconUI;
}
