// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bar.generated.h"

USTRUCT(BlueprintType)
struct WORKSHOP_API FBar
{
  GENERATED_BODY()

  // Sorted array which defines in which value bar is active.
  UPROPERTY(EditDefaultsOnly)
  TArray<int32> BarLimits;

  UPROPERTY(EditDefaultsOnly)
  int32 DefaultValue = 0;

  UPROPERTY() int32 CurrentValue;
  UPROPERTY() bool bIsActive;

  void ChangeBarBy(int32 Value);

  void ResetBar();

  bool IsActive();
};
