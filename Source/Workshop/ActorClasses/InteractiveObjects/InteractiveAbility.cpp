// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveAbility.h"


FString AInteractiveAbility::GatherInformation() const
{
  FString AbilityInformation = Super::GatherInformation();

  for (TSubclassOf<UEffectData> EffectClass : UsedEffects)
  {
    AbilityInformation += Cast<UEffectData>(EffectClass->StaticClass())->GatherInformation(true, MainManager) + "\n";
  }

  return AbilityInformation + "\n";
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