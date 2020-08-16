// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveAbility.h"

void AInteractiveAbility::GatherInformation()
{
  Super::GatherInformation();
}

void AInteractiveAbility::ShowInfluences()
{

}

AInteractiveAbility::AInteractiveAbility()
{

}

AInteractiveAbility::AInteractiveAbility(
  AInteractiveCharacter* Owner) : CharacterOwner(Owner)
{

}

void AInteractiveAbility::CustomEffect_Implementation(AInteractiveObject* aim)
{

}

void AInteractiveAbility::ResolveAbility()
{
  CharacterOwner->PlayAnimation(AbilityAnimationId);
  
  //++++ apply movememt

  for (AInteractiveObject* DependingObject : InfluencesList)
  {
    CustomEffect(DependingObject);
  }

  ClearInflunces();
}