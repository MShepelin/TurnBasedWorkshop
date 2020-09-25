// Fill out your copyright notice in the Description page of Project Settings.

#include "ChangeStatEffectData.h"


UChangeStatEffectData::UChangeStatEffectData()
{

}

void UChangeStatEffectData::ResolveOn(AInteractiveObject* TargetObject)
{
  if (TargetObject->InteractiveDataCore.IntegerStats.Find(StatID))
  {
    TargetObject->InteractiveDataCore.IntegerStats[StatID] += EffectValue;
  }
  else if (bIsForciblyAdded)
  {
    TargetObject->InteractiveDataCore.IntegerStats.Add(StatID, EffectValue);
  }
}
