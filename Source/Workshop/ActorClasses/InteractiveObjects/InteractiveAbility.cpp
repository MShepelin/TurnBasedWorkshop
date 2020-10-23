// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveAbility.h"
#include "Workshop/Builders/BuildAbility.h"
#include "InteractiveCharacter.h"

void AInteractiveAbility::ShowInfluences() const
{
  Super::ShowInfluences();
}

AInteractiveAbility::AInteractiveAbility()
{
  InteractiveDataCore.InteractiveType = static_cast<int32>(EInteractiveType::Ability);

  AbilityPresentation = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("AbilitySprite"));
  AbilityPresentation->SetupAttachment(RootComponent);

  // y-order
  AbilityPresentation->SetRelativeLocation(FVector(0, -1, 0)); 
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

  CharacterOwner->PlayAnimation(AbilityDataCore.AbilityAnimationId);
  
  //++++ apply movememt

  for (AInteractiveObject* DependentObject : InfluencesOn)
  {
    CustomEffect(DependentObject);
  }

  ClearInflunces();
}

void AInteractiveAbility::PostInitProperties()
{
  Super::PostInitProperties();

  if (AbilityDataCore.IconScene)
  {
    AbilityPresentation->SetFlipbook(AbilityDataCore.IconScene);
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

  TArray<AInteractiveObject*> FoundObjects = MainManager->FindObjectsByCTsWithMask(AbilityDataCore.CTsToAffect, 1, AbilityDataCore.TargetTypeMask);

#if WITH_EDITOR
  UE_LOG(LogTemp, Warning, TEXT("%d targets found"), FoundObjects.Num());

  // Show what objects were found
  for (AInteractiveObject* FoundObject : FoundObjects)
  {
    DrawDebugLine(GetWorld(), GetActorLocation(), FoundObject->GetActorLocation(),
      DEBUG_COLOR, false, DebugTime);
  }
#endif

  MainManager->AwakeByCenterObject(FoundObjects);
}

void AInteractiveAbility::UnpickedAsCentral()
{
  Super::UnpickedAsCentral();

  MainManager->PutToSleepManagedObjects(MainManager);
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
  return AbilityDataCore.TargetTypeMask;
}

void AInteractiveAbility::UpdateEffects(ETurnPhase TurnPhase)
{
  for (size_t EffectIndex = AbilityDataCore.UsedEffects.Num() - 1; EffectIndex >= 0; EffectIndex--)
  {
    UEffectData* ChosenEffect = AbilityDataCore.UsedEffects[EffectIndex];

    if (ChosenEffect->TurnPhaseToResolve != TurnPhase)
    {
      continue;
    }

    if (!ChosenEffect->bIsBonusEffect)
    {
      // BonusEffects always go after basic effects.
      break;
    }

    ChosenEffect->DecreaseDuration();

    // Remove effect if it is no longer present
    if (!ChosenEffect->Duration)
    {
      AbilityDataCore.UsedEffects.Swap(EffectIndex, AbilityDataCore.UsedEffects.Num() - 1);
      AbilityDataCore.UsedEffects.Pop();
    }
  }
}

void AInteractiveAbility::CenterInCharacterOwner()
{
  check(CharacterOwner != nullptr);
  CharacterOwner->SetCentralAbility(this);
}

UTexture2D* AInteractiveAbility::GetIconUI() const
{
  return AbilityDataCore.IconUI;
}
