// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildAbility.h"

AInteractiveObject* UBuildAbility::AddAllEffectsToObject(AInteractiveObject* TargetObject, AInteractiveAbility* Ability)
{
  if (!(TargetObject && Ability))
  {
    UE_LOG(LogTemp, Error, TEXT("Invalid input for AddAllEffectsToObject function!"));
    return TargetObject;
  }

  if (TargetObject->GetInteractiveType() & static_cast<int32>(EInteractiveType::Ability))
  {
    AInteractiveAbility* TargetAbility = Cast<AInteractiveAbility>(TargetObject);
    while (!Ability->AbilityDataCore.EffectsToResolve.IsEmpty())
    {
      FEffectData Effect;
      Ability->AbilityDataCore.EffectsToResolve.Dequeue(Effect);
      Effect.bIsBonusEffect = true;
      TargetAbility->AbilityDataCore.EffectsToResolve.Enqueue(Effect);
      TargetAbility->AbilityDataCore.EffectsToReceive.Enqueue(Effect);
    }

    TargetObject->UpdateCharacterStatus();
    return TargetObject;
  }

  FEffectData Effect;
  while (Ability->AbilityDataCore.EffectsToResolve.Dequeue(Effect))
  {
    for (FBar& Stat : TargetObject->InteractiveDataCore.Stats)
    {
      if (Stat.StatID == Effect.StatID)
      {
        Stat.ChangeBarBy(Effect.EffectValue);
        break;
      }
    }
  }

  TargetObject->UpdateCharacterStatus();
  return TargetObject;
}
