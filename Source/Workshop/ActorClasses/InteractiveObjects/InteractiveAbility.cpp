// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveAbility.h"


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
  InteractiveType = EInteractiveType::Ability;
}

AInteractiveAbility::AInteractiveAbility(
  AInteractiveCharacter* Owner) : CharacterOwner(Owner)
{

}

void AInteractiveAbility::CustomEffect_Implementation(AInteractiveObject* TargetObject)
{
  UE_LOG(LogTemp, Warning, TEXT("Ability has no implementation, change CustomEffect to add it"));
}

void AInteractiveAbility::ResolveAbility()
{
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

  for (size_t ObjectIndex = 0; ObjectIndex < MainManager->FoundObjects.Num(); ObjectIndex++)
  {
    AInteractiveObject* FoundObject = MainManager->FoundObjects[ObjectIndex];

    if ((FoundObject->GetInteractiveType() & TargetTypeMask) != TargetTypeMask)
    {
      MainManager->FoundObjects.Swap(ObjectIndex, FoundObjects.Num() - 1);
      MainManager->FoundObjects.Pop();
    }
  }

#if WITH_EDITOR
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
