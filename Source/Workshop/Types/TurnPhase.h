// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurnPhase.generated.h"


UENUM(BlueprintType)
enum class ETurnPhase : uint8
{
  Start = 0,
  AbilitiesSetup = 1,
  AbilitiesEffect = 2,
  End = 3

  //???? add update phases
};
