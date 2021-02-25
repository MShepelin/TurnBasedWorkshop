// Copyright © Dmitriy Shepelin 2021. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveType.generated.h"


UENUM(meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EInteractiveType : uint8
{
  Nothing             = 0,
  Ability             = 1,
  Character           = 2,
  Item                = 4,
  PlayerControlled    = 8, 
  NotPlayerControlled = 16,
  Exhausted           = 32
};

// In search all bits starting from log(INTERACTIVE_TYPE_SEPARATOR_MASK)+1
// must meet and at least one of first log(INTERACTIVE_TYPE_SEPARATOR_MASK)

ENUM_CLASS_FLAGS(EInteractiveType)