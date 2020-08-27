// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveType.generated.h"


UENUM(BlueprintType)
enum class EInteractiveType : uint8
{
  Nothing   = 0,
  Ability   = 1,
  Character = 2,
  Any       = 3,
};
