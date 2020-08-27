// Fill out your copyright notice in the Description page of Project Settings.

#include "AdvantageEffectData.h"


UAdvantageEffectData::UAdvantageEffectData()
{
  EffectType = EEffectType::Advantage;
}


FString UAdvantageEffectData::GatherInformation(bool bIsAbilityInfo, ARegistrationManager* Manager) const
{
  FString EffectInforamtion = Super::GatherInformation(bIsAbilityInfo, Manager);
  EffectInforamtion += "gets " + UEnum::GetValueAsString(AdvantageType).RightChop(LengthOfAdvantageTypeName) + " from";

  if (!AdvantageCTs.Num())
  {
    EffectInforamtion += " everything\n";
    return EffectInforamtion;
  }

  for (int32 AdvantageCT : AdvantageCTs)
  {
    EffectInforamtion += " " + Manager->GetCTName(AdvantageCT);
  }

  return EffectInforamtion + "\n";
}
