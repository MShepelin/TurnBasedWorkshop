// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "EffectSpecifiers.generated.h"

UENUM(meta = (Bitflags))
enum class EEffectSpecifiers : int32
{
  Accident    = 1,
  Exhaust     = 2,
  Plan        = 4,
  Luck        = 8,
  Fear        = 16,
  Boredom     = 32,
  Madness     = 64
};

ENUM_CLASS_FLAGS(EEffectSpecifiers)