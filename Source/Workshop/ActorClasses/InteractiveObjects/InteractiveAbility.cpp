// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveAbility.h"


FString AInteractiveAbility::GatherInformation() const
{
  FString AbilityInformation = Super::GatherInformation();

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