// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildAbility.h"


AInteractiveObject* UBuildAbility::AddEffectToObject(AInteractiveObject* TargetObject, AInteractiveAbility* Ability, int32 EffectIndex)
{
  if (!IsValid(TargetObject) || !IsValid(Ability))
  {
    UE_LOG(LogTemp, Error, TEXT("Invalid input!"));
    return TargetObject;
  }

  if (EffectIndex < 0 || EffectIndex < Ability->UsedEffects.Num())
  {
    UE_LOG(LogTemp, Error, TEXT("Wrong Index!"));
    return TargetObject;
  }

  UEffectData* EffectDublicate = DuplicateObject(Ability->UsedEffects[EffectIndex], TargetObject);
  TargetObject->AccumulatedEffects.Add(EffectDublicate);
  if (EffectDublicate->EffectType == EEffectType::Advantage)
  {
    TargetObject->SetOfAdvantages.Add(EffectDublicate);
  }

  return TargetObject;
}

AInteractiveObject* UBuildAbility::AddAllEffectsToObject(AInteractiveObject* TargetObject, AInteractiveAbility* Ability)
{
  if (!IsValid(TargetObject) || !IsValid(Ability))
  {
    UE_LOG(LogTemp, Error, TEXT("Invalid input!"));
    return TargetObject;
  }

  for (UEffectData* Effect : Ability->UsedEffects)
  {
    if (!Effect->Duration)
    {
      switch (Effect->EffectType)
      {
      case EEffectType::StatChange:
      {
        UChangeStatEffectData* StatChangeEffect = Cast< UChangeStatEffectData>(Effect);
        if (TargetObject->IntegerStats.Find(StatChangeEffect->StatID))
        {
          if (StatChangeEffect->bIsRelative)
          {
            //???? check if StatChangeEffect->EffectValue > 100
            TargetObject->IntegerStats[StatChangeEffect->StatID] = FGenericPlatformMath::RoundToInt(
              TargetObject->IntegerStats[StatChangeEffect->StatID] *
              (1. - ((float)StatChangeEffect->EffectValue) / 100)
            );
          }
          else
          {
            TargetObject->IntegerStats[StatChangeEffect->StatID] += StatChangeEffect->EffectValue;
          }
        }
        break;
      }
      default:
        break;
      }
      
    }

    TargetObject->AccumulatedEffects.Add(DuplicateObject(Effect, TargetObject));
  }

  return TargetObject;
}
