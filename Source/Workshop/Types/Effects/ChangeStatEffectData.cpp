// Fill out your copyright notice in the Description page of Project Settings.

#include "ChangeStatEffectData.h"


UChangeStatEffectData::UChangeStatEffectData()
{
  EffectType = EEffectType::StatChange;
}


FString UChangeStatEffectData::GatherInformation(bool bIsAbilityInfo, ARegistrationManager* Manager) const
{
  FString EffectInforamtion = Super::GatherInformation(bIsAbilityInfo, Manager);

  EffectInforamtion += "gets " + Manager->GetStatNameByID(StatID) + " ";

  if (EffectValue > 0)
  {
    EffectInforamtion += "+";
  }

  EffectInforamtion += FString::FromInt(EffectValue) + "\n";
  
  return EffectInforamtion;
}
