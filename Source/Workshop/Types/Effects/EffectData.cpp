// Fill out your copyright notice in the Description page of Project Settings.

#include "EffectData.h"


UEffectData::UEffectData()
{

}

void UEffectData::DecreaseDuration(int32 OnValue)
{
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

int UEffectData::GetDuration() const
{
  return Duration;
}

EEffectType UEffectData::GetEffectType() const
{
  return EffectType;
}

EInteractiveType UEffectData::GetTargetType() const
{
  return TargetType;
}

int32 UEffectData::GetEffectSpecifiers() const
{
  return EffectSpecifiersMask;
}