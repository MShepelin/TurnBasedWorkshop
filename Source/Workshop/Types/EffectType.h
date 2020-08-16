#pragma once

#include "CoreMinimal.h"
#include "EffectType.generated.h"

UENUM(BlueprintType)
enum EEffectType
{
  Advantage   UMETA(DisplayName = "Advantage"),
  StatChange  UMETA(DisplayName = "StatChange"),
  Spawn       UMETA(DisplayName = "Spawn")
};