// Copyright © Dmitriy Shepelin 2021. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "TurnPhase.generated.h"


UENUM(BlueprintType)
enum class ETurnPhase : uint8
{
  Start = 0,
  End = 1
};
