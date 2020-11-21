// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bar.generated.h"

USTRUCT(BlueprintType)
struct WORKSHOP_API FBar
{
  GENERATED_BODY()

  // Sorted array which defines in which value bar is active.
  // Values are from 0 and 1.
  UPROPERTY(EditDefaultsOnly, meta = (ClampMin="0.0", ClampMax="1.0"))
  TArray<float> BarLimits;

  UPROPERTY(EditDefaultsOnly)
  float DefaultValue = 0;

  UPROPERTY(BlueprintReadWrite) float CurrentValue;
  UPROPERTY(BlueprintReadWrite) bool bIsActive;

  void ChangeBarBy(float Value);

  void ResetBar();

  bool IsActive();
};
