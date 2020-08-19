// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "EffectType.generated.h"

UENUM(BlueprintType)
enum class EEffectType : uint8
{
  DataHolder          = 0,
  Advantage           = 1 UMETA(DisplayName = "Advantage"),
  StatChange          = 2 UMETA(DisplayName = "StatChange"),
  RelativeStatChange  = 4 UMETA(DisplayName = "RelativeStatChange"),
  Spawn               = 8 UMETA(DisplayName = "Spawn")
};