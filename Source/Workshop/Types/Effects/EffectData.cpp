// Fill out your copyright notice in the Description page of Project Settings.

#include "EffectData.h"


UEffectData::UEffectData()
{

}

UEffectData::UEffectData(
  FString AspectNameInput, 
  EEffectType EffectTypeInput,
  EInteractiveType TargetTypeInput, 
  int32 DurationInput = 0, 
  int32 EffectValueInput = 0,
  TSubclassOf<AInteractiveObject> SpawnTypeInput = nullptr, 
  int32 EffectIdentifier = 0, int32 EffectSpecifiersMaskInput = 1) :
  AspectName(AspectNameInput), EffectType(EffectTypeInput), 
  TargetType(TargetTypeInput), Duration(DurationInput), 
  EffectValue(EffectValueInput), SpawnType(SpawnTypeInput), 
  EffectId(EffectIdentifier), EffectSpecifiersMask(EffectSpecifiersMaskInput)
{

}

void UEffectData::DecreaseDuration(int32 OnValue)
{
  if (OnValue < 0)
  {
    UE_LOG(LogTemp, Warning, TEXT("Can't use negative value here!!!"));
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

int UEffectData::GetEffectId() const
{
  return EffectId;
}

int UEffectData::GetDuration() const
{
  return Duration;
}

int UEffectData::GetSpecialValue() const
{
  return EffectValue;
}

FString UEffectData::GetAspectName() const
{
  return AspectName;
}

EEffectType UEffectData::GetEffectType() const
{
  return EffectType;
}

TSubclassOf<AInteractiveObject> UEffectData::GetSpawnType() const
{
  return SpawnType;
}

EInteractiveType UEffectData::GetTargetType() const
{
  return TargetType;
}

int32 UEffectData::GetEffectSpecifiers() const
{
  return EffectSpecifiersMask;
}