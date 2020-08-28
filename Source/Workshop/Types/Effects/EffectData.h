// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <algorithm>
#include "CoreMinimal.h"
#include "Workshop/Types/EffectType.h"
#include "Workshop/Types/EffectSpecifiers.h"
#include "Workshop/Types/InteractiveType.h"
#include "Workshop/Types/TurnPhase.h"
#include "Workshop/ActorClasses/Managers/RegistrationManager.h"
#include "EffectData.generated.h"


UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class WORKSHOP_API UEffectData : public UObject
{
  GENERATED_BODY()

public:
  EEffectType EffectType = EEffectType::DataHolder;

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
  EInteractiveType TargetType;

  // Duration of effect: 
  // 0 means that effect is applied when added, 
  // -1 means that effect is always present on the object,
  // other values are used depending on the ActiveOn parameter.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings", meta = (ClampMin = "-1"))
  int32 Duration = 0;

  // Set the phase in start of which this affect will be applied.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings")
  ETurnPhase ActiveOn = ETurnPhase::AbilitiesEffect;

  // Effect Specifiers gather Effects in groups so that they can be changed together.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings", meta = (Bitmask, BitmaskEnum = "EEffectSpecifiers"))
  int32 EffectSpecifiersMask = 0;

  // Types of objects which are affected by this effect.
  // Affects everithing if empty.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings")
  TArray<int32> CTsToAffect;

  UEffectData();

  void DecreaseDuration(int32 OnValue);
  void DecreaseDuration();

  virtual FString GatherInformation(bool bIsAbilityInfo, ARegistrationManager* Manager) const;
};
