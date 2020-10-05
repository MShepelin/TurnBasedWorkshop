// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "../CharacterStatus.h"
#include "CharacterCore.generated.h"

class AInteractiveAbility;

USTRUCT(BlueprintType)
struct WORKSHOP_API FCharacterCore
{
  GENERATED_BODY()

  // Abilities which Interactive character can use.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterSettings")
  TArray<TSubclassOf<AInteractiveAbility>> AbilitiesClasses;

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterSettings", meta = (Bitmask, BitmaskEnum = "ECharacterStatus"))
  uint8 CharacterStatus = static_cast<uint8>(ECharacterStatus::NoStatus);

  // Map of animations with their integer identifiers.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterSettings")
  TMap<int32, UPaperFlipbook*> AnimationsMap;
};
