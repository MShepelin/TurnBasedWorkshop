// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "EffectSpecifiers.generated.h"

UENUM(meta = (Bitflags))
enum class EEffectSpecifiers : uint32
{
  Accident    = 0x01,
  Exhaust     = 0x02,
  Plan        = 0x04,
  Luck        = 0x08,
  Fear        = 0x10,
};

ENUM_CLASS_FLAGS(EEffectSpecifiers)