// Fill out your copyright notice in the Description page of Project Settings.

#include "EffectData.h"

int32 FEffectData::DecreaseDuration(int32 OnValue)
{
  if (Duration == -1)
  {
    return Duration;
  }

  if (OnValue < 0)
  {
    UE_LOG(LogTemp, Error, TEXT("Can't use negative value here!!!"));
    return Duration;
  }

  if (OnValue >= Duration)
  {
    Duration = 0;
  }
  else
  {
    Duration -= OnValue;
  }

  return Duration;
}

int32 FEffectData::DecreaseDuration()
{
  return DecreaseDuration(1);
}

FString FEffectData::GetInfoString(ARegistrationManager* UsedManager)
{
  FString InfoString = UsedManager->GetStatNameByID(StatID) + " " + ((EffectValue > 0) ? "+" : "") + " " + FString::FromInt(EffectValue);

  if (Duration == -1)
  {
    InfoString += "(inf)";
  }
  else if (Duration)
  {
    InfoString += "(" + FString::FromInt(Duration) + ")";
  }

  return InfoString;
}
