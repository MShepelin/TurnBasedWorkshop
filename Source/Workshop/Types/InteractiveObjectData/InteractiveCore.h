// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Effects/EffectData.h"
#include "../Bar.h"
#include "../InteractiveType.h"
#include "InteractiveCore.generated.h"

USTRUCT(BlueprintType)
struct WORKSHOP_API FInteractiveCore
{
  GENERATED_BODY()

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "InteractiveSettings", meta = (Bitmask, BitmaskEnum = "EInteractiveType"))
  uint8 InteractiveType;

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "InteractiveSettings", meta = (ClampMin = "1"));
  TArray<int32> CTsOfObject;

  // Name to display.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "InteractiveSettings")
  FName InteractiveName;

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "InteractiveSettings")
  TArray<FBar> Stats;

  FCriticalSection GettingStats;

  TArray<FBar> GetStats();

  FInteractiveCore();
  FInteractiveCore(const FInteractiveCore & AnotherBar);
  FInteractiveCore& operator=(const FInteractiveCore & AnotherCore);
};
