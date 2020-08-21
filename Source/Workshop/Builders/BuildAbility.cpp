// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildAbility.h"

AInteractiveObject* UBuildAbility::AddEffectToCharacter(AInteractiveCharacter* Aim, AInteractiveAbility* Ability, int32 EffectIndex)
{
  Aim->AccumulatedEffects.Add(NewObject<UEffectData>(Aim, Ability->UsedEffects[EffectIndex]));

  return Aim;
}