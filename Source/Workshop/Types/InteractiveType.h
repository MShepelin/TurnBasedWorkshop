// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveType.generated.h"


UENUM(BlueprintType)
enum class EInteractiveType : uint8
{
  Base        UMETA(DisplayName = "Any"),
  Ability     UMETA(DisplayName = "Ability"),
  Character   UMETA(DisplayName = "Character"),
};
