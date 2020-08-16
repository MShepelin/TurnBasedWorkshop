// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <algorithm>
#include "CoreMinimal.h"
#include "Workshop/Types/EffectType.h"
#include "Workshop/Types/InteractiveType.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveObject.h"
#include "EffectData.generated.h"

UCLASS(Blueprintable, BlueprintType)
class WORKSHOP_API UEffectData : public UObject
{
  GENERATED_BODY()

protected:
  // Name of aspect (property) to change in target, also used to put name for Advantage to apply
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (OverrideNativeName = "AspectToChange"))
  FString AspectName = "";

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (OverrideNativeName = "EffectType"))
  EEffectType EffectType = EEffectType::StatChange;

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (OverrideNativeName = "TargetType"))
  EInteractiveType TargetType = EInteractiveType::Character;

  // Duration of effect, 0 means that effect is permanent
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta= (OverrideNativeName = "Duration", ClampMin = "0"))
  int Duration = 0;

  // Used only with Spawn/StatChange EffectType
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (OverrideNativeName = "ValueToUse"))
  int EffectValue = 0;

  // Used only with Spawn EffectType
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (OverrideNativeName = "ObjectToSpawn"))
  TSubclassOf<AInteractiveObject> SpawnType = nullptr;

  // Is used to gather same effects
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (OverrideNativeName = "EffectIdentifier"))
  int EffectId = 0;

  // Effect Specifiers gather Effects in groups so that they can be changed together
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (OverrideNativeName = "EffectIdentifier"))
  int EffectSpecifiersMask = 0;

  // tag types to affect on

public:
  UEffectData();

  UEffectData(FString AspectNameInput, TEnumAsByte<EEffectType> EffectTypeInput,
    TEnumAsByte <EInteractiveType> TargetTypeInput, int DurationInput, int EffectValueInput,
    TSubclassOf<AInteractiveObject> SpawnTypeInput, int EffectIdentifier);

  void DecreaseDuration(int OnValue);
  void DecreaseDuration();

  int GetEffectId();
  int GetDuration();
  int GetSpecialValue();
  FString GetAspectName();
  TEnumAsByte<EEffectType> GetEffectType();
  TSubclassOf<AInteractiveObject> GetSpawnType();
  TEnumAsByte <EInteractiveType> GetTargetType();
};
