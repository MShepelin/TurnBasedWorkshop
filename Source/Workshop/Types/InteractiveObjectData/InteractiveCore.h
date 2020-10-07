// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Effects/EffectData.h"
#include "InteractiveCore.generated.h"

USTRUCT(BlueprintType)
struct WORKSHOP_API FInteractiveCore
{
  GENERATED_BODY()

  // ---------------------- //
  // Interactive Properties //
  // ---------------------- //

  UPROPERTY() int32 InteractiveType = static_cast<int32>(EInteractiveType::Nothing);

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "InteractiveSettings", meta = (ClampMin = "1"));
  TArray<int32> CTsOfObject;

  // ----------------- //
  // Object Statistics //
  // ----------------- //

  // Statisctics in form of strings
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "InteractiveSettings")
  FName InteractiveName;

  // Statisctics in form of integers
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "InteractiveSettings")
  TMap<int32, int32> IntegerStats;

  // Array of effects which are applied in the current state.
  TMap<ETurnPhase, TArray<UEffectData*>> AccumulatedEffects;
};
