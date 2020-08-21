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
  EEffectType EffectType = EEffectType::DataHolder;

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
  EInteractiveType TargetType;

  // Duration of effect, 0 means that effect is permanent.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings", meta = (ClampMin = "0"))
  int32 Duration = 0;

  // Effect Specifiers gather Effects in groups so that they can be changed together.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings", meta = (Bitmask, BitmaskEnum = "EEffectSpecifiers"))
  int32 EffectSpecifiersMask;

  // Types of objects which are affected by this effect.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings")
  TArray<int32> TagsToAffect;

  //++++ add option to edit tags, which shouldn't be used.

public:
  UEffectData();

  void DecreaseDuration(int32 OnValue);
  void DecreaseDuration();

  int32 GetDuration() const;
  int32 GetEffectSpecifiers() const;
  EEffectType GetEffectType() const;
  EInteractiveType GetTargetType() const;
};
