// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <algorithm>
#include "CoreMinimal.h"
#include "Workshop/Types/EffectType.h"
#include "Workshop/Types/EffectSpecifiers.h"
#include "Workshop/Types/InteractiveType.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveObject.h"
#include "EffectData.generated.h"

UCLASS(Blueprintable, BlueprintType)
class WORKSHOP_API UEffectData : public UObject
{
  GENERATED_BODY()

protected:
  // Name of aspect (property) to change in target, also used to put name for Advantage to apply
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings", meta = (OverrideNativeName = "AspectToChange"))
  FString AspectName = "";

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
  EEffectType EffectType = EEffectType::StatChange;

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
  EInteractiveType TargetType = EInteractiveType::Character;

  // Duration of effect, 0 means that effect is permanent
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings", meta= (ClampMin = "0"))
  int Duration = 0;

  // Used only with Spawn/StatChange EffectType
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings", meta = (OverrideNativeName = "ValueToUse"))
  int EffectValue = 0;

  // Used only with Spawn EffectType
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings", meta = (OverrideNativeName = "ObjectToSpawn"))
  TSubclassOf<AInteractiveObject> SpawnType = nullptr;

  // Is used to gather same effects
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings", meta = (OverrideNativeName = "EffectIdentifier"))
  int EffectId = 0;

  // Effect Specifiers gather Effects in groups so that they can be changed together
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings", meta =(Bitmask, BitmaskEnum = "EEffectSpecifiers"))
  int32 EffectSpecifiersMask;

  // Types of objects which are affected by this effect
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings")
  TArray<int> TagsToAffect;

  // Desription in Tags system should be done in some sort of document
  //++++ add option to edit tags, which shouldn't be used
  //++++ add editor button to add all existing tags

public:
  UEffectData();

  UEffectData(FString AspectNameInput, EEffectType EffectTypeInput,
    EInteractiveType TargetTypeInput, int DurationInput, int EffectValueInput,
    TSubclassOf<AInteractiveObject> SpawnTypeInput, int EffectIdentifier, int32 EffectSpecifiersMaskInput);

  void DecreaseDuration(int OnValue);
  void DecreaseDuration();

  int GetEffectId();
  int GetDuration();
  int GetSpecialValue();
  FString GetAspectName();
  int32 GetEffectSpecifiers();
  EEffectType GetEffectType();
  EInteractiveType GetTargetType();
  TSubclassOf<AInteractiveObject> GetSpawnType();
};
