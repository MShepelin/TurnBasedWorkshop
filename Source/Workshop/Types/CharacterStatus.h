// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStatus.generated.h"


UENUM(meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ECharacterStatus : uint8
{
  NoStatus     = 0,
  Exhaust      = 1,  // Stamina ran out
  Impatience   = 2,  // Patience ran out
  Hostility    = 4,  // Amity ran out
  Distrust     = 8,  // Trust ran out
  Hindsight    = 16, // Sagacity ran out
  Sorrow       = 32, // Tune ran out
  Stupor       = 64, // Creativity ran out
  Disrespect   = 128 // Respect ran out
};

ENUM_CLASS_FLAGS(ECharacterStatus)
