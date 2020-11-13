// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurnPhase.generated.h"


UENUM(BlueprintType)
enum class ETurnPhase : uint8
{
  Start = 0,
  AbilitiesEffect = 1,
  End = 2
};
