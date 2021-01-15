// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveAbility.h"
#include "Workshop/Builders/BuildAbility.h"
#include "InteractiveCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Workshop/GameControl/PlayerControllers/InteractController.h"

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
  check(CharacterOwner != nullptr);

  for (FEffectData Effect : AbilityDataCore.UsedEffects)
  {
    AbilityDataCore.EffectsToResolve.Enqueue(Effect);
  }

  CharacterOwner->PlayAnimation(AbilityDataCore.AbilityAnimationId, true);
  //++++ add movement

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
      DEBUG_COLOR, false, DEBUG_TIME);
  }
#endif

  MainManager->AwakeByCenterObject(FoundObjects);

  // Add list of abilities to the AbilitiesWidget.
  UAbilitiesWidget* AbilitiesWidget = Cast<AInteractController>(
    UGameplayStatics::GetPlayerController(GetWorld(), 0))->GetAbilitiesWidget();
  if (AbilitiesWidget)
  {
    AbilitiesWidget->FillAbilitySlots({ this }, false);
    AbilitiesWidget->FillBarSlots(InteractiveDataCore.Stats);

    AbilitiesWidget->ShowAbilitySlots();
    AbilitiesWidget->ShowBarsSlots();
  }
}

void AInteractiveAbility::UnpickedAsCentral()
{
  Super::UnpickedAsCentral();

  // Remove the ability from AbilitiesWidget.
  UAbilitiesWidget* AbilitiesWidget = Cast<AInteractController>(
    UGameplayStatics::GetPlayerController(GetWorld(), 0))->GetAbilitiesWidget();
  if (AbilitiesWidget)
  {
    AbilitiesWidget->HideAbilitySlots();
    AbilitiesWidget->HideBarsSlots();
  }

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

void AInteractiveAbility::CenterInCharacterOwner()
{
  check(CharacterOwner != nullptr);
  CharacterOwner->SetCentralAbility(this);
}

UTexture2D* AInteractiveAbility::GetIconUI() const
{
  return AbilityDataCore.IconUI;
}

void AInteractiveAbility::UpdateCharacterStatus()
{
  // Receive additional effects.
  while (!AbilityDataCore.EffectsToReceive.IsEmpty())
  {
    AbilityDataCore.UsedEffects.Add(FEffectData());
    AbilityDataCore.EffectsToReceive.Dequeue(AbilityDataCore.UsedEffects.Last());
  }

  for (int EffectIndex = AbilityDataCore.UsedEffects.Num() - 1; EffectIndex >= 0; EffectIndex--)
  {
    FEffectData& ChosenEffect = AbilityDataCore.UsedEffects[EffectIndex];

    // BonusEffects always go after basic effects.
    if (!ChosenEffect.bIsBonusEffect)
      break;

    // Remove effect if it is no longer present
    if (!ChosenEffect.DecreaseDuration())
      AbilityDataCore.UsedEffects.RemoveAtSwap(EffectIndex);
  }
}
