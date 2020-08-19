// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectData.h"

UEffectData::UEffectData()
{

}

UEffectData::UEffectData(
  FString AspectNameInput, 
  EEffectType EffectTypeInput,
  EInteractiveType TargetTypeInput, 
  int DurationInput = 0, 
  int EffectValueInput = 0,
  TSubclassOf<AInteractiveObject> SpawnTypeInput = nullptr, 
  int EffectIdentifier = 0, int32 EffectSpecifiersMaskInput = 1) :
  AspectName(AspectNameInput), EffectType(EffectTypeInput), 
  TargetType(TargetTypeInput), Duration(DurationInput), 
  EffectValue(EffectValueInput), SpawnType(SpawnTypeInput), 
  EffectId(EffectIdentifier), EffectSpecifiersMask(EffectSpecifiersMaskInput)
{

}

void UEffectData::DecreaseDuration(int OnValue)
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

int UEffectData::GetEffectId()
{
  return EffectId;
}

int UEffectData::GetDuration()
{
  return Duration;
}

int UEffectData::GetSpecialValue()
{
  return EffectValue;
}

FString UEffectData::GetAspectName()
{
  return AspectName;
}

EEffectType UEffectData::GetEffectType()
{
  return EffectType;
}

TSubclassOf<AInteractiveObject> UEffectData::GetSpawnType()
{
  return SpawnType;
}

EInteractiveType UEffectData::GetTargetType()
{
  return TargetType;
}

int32 UEffectData::GetEffectSpecifiers()
{
  return EffectSpecifiersMask;
}