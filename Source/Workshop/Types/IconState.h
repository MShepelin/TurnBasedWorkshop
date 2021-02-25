// Copyright © Dmitriy Shepelin 2021. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "IconState.generated.h"


UENUM(BlueprintType)
enum class EIconState : uint8
{
  AvailableTarget = 0,
  ChosenTarget = 1,
  CentralObject = 2
};
