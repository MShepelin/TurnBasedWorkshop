// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveType.generated.h"


UENUM(meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EInteractiveType : int32
{
  Nothing   = 0,
  Ability   = 1,
  Character = 2,
};

ENUM_CLASS_FLAGS(EInteractiveType)