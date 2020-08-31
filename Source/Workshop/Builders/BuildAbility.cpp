// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildAbility.h"

AInteractiveObject* UBuildAbility::AddAllEffectsToObject(AInteractiveObject* TargetObject, AInteractiveAbility* Ability)
{
  //???? may be just if(!(TargetObject && Ability))
  if (!IsValid(TargetObject) || !IsValid(Ability))
  {
    UE_LOG(LogTemp, Error, TEXT("Invalid input for AddAllEffectsToObject function!"));
    return TargetObject;
  }

  for (UEffectData* Effect : Ability->UsedEffects)
  {
    // Apply instant effect
    if (!Effect->Duration)
    {
      Effect->ResolveOn(TargetObject);
      continue;
    }

    // Transfer temporary effect
    UEffectData* DublicatedEffect = DuplicateObject(Effect, TargetObject);
    DublicatedEffect->bIsBonusEffect = true;

    if ((TargetObject->InteractiveType == static_cast<int32>(EInteractiveType::Ability)) && Effect->bIsAmbiguous)
    {
      Cast<AInteractiveAbility>(TargetObject)->UsedEffects.Add(DublicatedEffect);
    }
    else
    {
      TargetObject->AccumulatedEffects.Add(DublicatedEffect);
    }
  }

  return TargetObject;
}
