// Fill out your copyright notice in the Description page of Project Settings.

#include "EffectData.h"


UEffectData::UEffectData()
{

}

void UEffectData::DecreaseDuration(int32 OnValue)
{
  if (Duration == -1)
  {
    return;
  }

  if (OnValue < 0)
  {
    UE_LOG(LogTemp, Error, TEXT("Can't use negative value here!!!"));
    return;
  }

  if (OnValue >= Duration)
  {
    Duration = 0;
  }
  else
  {
    Duration -= OnValue;
  }
}

void UEffectData::DecreaseDuration()
{
  DecreaseDuration(1);
}

/* REMAKE
FString UEffectData::GatherInformation(bool bIsAbilityInfo, ARegistrationManager* Manager) const
{
  if (!bIsAbilityInfo)
  {
    return "";
  }

  if (!Manager)
  {
    UE_LOG(LogTemp, Error, TEXT("Can't gather information without correct manager!"));
    return "";
  }

  FString EffectInforamtion = "";

  return EffectInforamtion;
}
*/

void UEffectData::ResolveOn(AInteractiveObject* TargetObject)
{

}
