// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildAbility.h"


AInteractiveObject* UBuildAbility::AddEffectToObject(AInteractiveObject* TargetObject, AInteractiveAbility* Ability, int32 EffectIndex)
{
  //++++ check if pointers are valid

  if (EffectIndex < 0 || EffectIndex < Ability->UsedEffects.Num())
  {
    UE_LOG(LogTemp, Error, TEXT("Wrong Index!"));
    return TargetObject;
  }

  TargetObject->AccumulatedEffects.Add(NewObject<UEffectData>(TargetObject, Ability->UsedEffects[EffectIndex]));

  return TargetObject;
}