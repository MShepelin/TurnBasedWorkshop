// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/ScopeLock.h"
#include "HAL/ThreadSafeBool.h"
#include "Bar.generated.h"

USTRUCT(BlueprintType)
struct WORKSHOP_API FBar
{
  GENERATED_BODY()

  // Sorted array which defines between which values the bar is active.
  // Active and inactive zones alternate, but the first zone is always inactive.
  // Values are in the range from 0 and 100.
  // WARNING: Value 0 and value 100 must be included.
  UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0", ClampMax = "100"))
  TArray<int8> BarLimits;

  UPROPERTY(EditDefaultsOnly)
  int8 DefaultValue = 0;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  int32 StatID;

  UPROPERTY() 
  int8 CurrentValue;

  FThreadSafeBool bIsActive;

  FCriticalSection Change;

  void ChangeBarBy(int8 Value);
  void ResetBar();
  bool IsActive() const;

  FBar();

  FBar(const FBar & AnotherBar);

  FBar& operator=(const FBar & AnotherBar);
};
