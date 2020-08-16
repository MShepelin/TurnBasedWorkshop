// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "EffectType.generated.h"

UENUM(BlueprintType)
enum class EEffectType : uint8
{
  Advantage   UMETA(DisplayName = "Advantage"),
  StatChange  UMETA(DisplayName = "StatChange"),
  Spawn       UMETA(DisplayName = "Spawn")
};