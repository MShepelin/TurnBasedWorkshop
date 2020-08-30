// Fill out your copyright notice in the Description page of Project Settings.

#include "ChangeStatEffectData.h"


UChangeStatEffectData::UChangeStatEffectData()
{

}

FString UChangeStatEffectData::GatherInformation(bool bIsAbilityInfo, ARegistrationManager* Manager) const
{
  FString EffectInforamtion = Super::GatherInformation(bIsAbilityInfo, Manager);

  EffectInforamtion += Manager->GetStatNameByID(StatID) + " ";

  if (EffectValue > 0)
  {
    EffectInforamtion += "+";
  }

  EffectInforamtion += FString::FromInt(EffectValue);
  
  return EffectInforamtion + "\n";
}

void UChangeStatEffectData::ResolveOn(AInteractiveObject* TargetObject)
{
  if (TargetObject->IntegerStats.Find(StatID))
  {
    TargetObject->IntegerStats[StatID] += EffectValue;
  }
  else if (bIsForciblyAdded)
  {
    TargetObject->IntegerStats.Add(StatID, EffectValue);
  }
}
