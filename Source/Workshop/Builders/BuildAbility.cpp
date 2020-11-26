// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildAbility.h"

AInteractiveObject* UBuildAbility::AddAllEffectsToObject(AInteractiveObject* TargetObject, AInteractiveAbility* Ability)
{
  if (!(TargetObject && Ability))
  {
    UE_LOG(LogTemp, Error, TEXT("Invalid input for AddAllEffectsToObject function!"));
    return TargetObject;
  }

  for (UEffectData* Effect : Ability->AbilityDataCore.UsedEffects)
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

    if (TargetObject->GetInteractiveType() & static_cast<int32>(EInteractiveType::Ability))
    {
      Cast<AInteractiveAbility>(TargetObject)->AbilityDataCore.UsedEffects.Add(DublicatedEffect);
    }
  }

  return TargetObject;
}
