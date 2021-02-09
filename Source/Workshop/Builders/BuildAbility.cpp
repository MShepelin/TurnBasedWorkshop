// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildAbility.h"

void UBuildAbility::AddAllEffectsToObject(const TArray<FEffectData>& Effects, AInteractiveObject* TargetObject)
{
  // Check if we should apply special logic to TargetObject
  AInteractiveAbility* TargetAbility = Cast<AInteractiveAbility>(TargetObject);
  if (TargetAbility && (TargetObject->GetInteractiveType() & static_cast<int32>(EInteractiveType::Ability)))
  {
    for (const FEffectData& Effect : Effects)
    {
      TargetAbility->AbilityDataCore.EffectsToResolve.Enqueue(Effect);
      TargetAbility->AbilityDataCore.EffectsToReceive.Enqueue(Effect);
    }

    // Don't UpdateExhaust of the ability because it is done by a controller in the end of resolution
    return;
  }

  for (const FEffectData& Effect : Effects)
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

  TargetObject->UpdateExhaust();
}

TArray<FEffectData> UBuildAbility::GatherEffects(AInteractiveAbility* Ability)
{
  TArray<FEffectData> ResolvedEffects;
  while (!Ability->AbilityDataCore.EffectsToResolve.IsEmpty())
  {
    ResolvedEffects.Add(FEffectData());
    Ability->AbilityDataCore.EffectsToResolve.Dequeue(ResolvedEffects.Last());
    ResolvedEffects.Last().bIsBonusEffect = true;
  }

  return ResolvedEffects;
}

TArray<AInteractiveObject*> UBuildAbility::GetTargets(const AInteractiveAbility* Ability)
{
  TArray<AInteractiveObject*> Targets;
  for (AInteractiveObject* Target : Ability->InfluencesOn)
  {
    Targets.Add(Target);
  }

  return Targets;
}
