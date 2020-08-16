#include "EffectData.h"

UEffectData::UEffectData()
{

}

UEffectData::UEffectData(
  FString AspectNameInput, 
  TEnumAsByte<EEffectType> EffectTypeInput,
  TEnumAsByte <EInteractiveType> TargetTypeInput, 
  int DurationInput = 0, 
  int EffectValueInput = 0,
  TSubclassOf<AInteractiveObject> SpawnTypeInput = nullptr, 
  int EffectIdentifier = 0) :
  AspectName(AspectNameInput), EffectType(EffectTypeInput), 
  TargetType(TargetTypeInput), Duration(DurationInput), 
  EffectValue(EffectValueInput), SpawnType(SpawnTypeInput), 
  EffectId(EffectIdentifier)
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

TEnumAsByte<EEffectType> UEffectData::GetEffectType()
{
  return EffectType;
}

TSubclassOf<AInteractiveObject> UEffectData::GetSpawnType()
{
  return SpawnType;
}

TEnumAsByte <EInteractiveType> UEffectData::GetTargetType()
{
  return TargetType;
}
