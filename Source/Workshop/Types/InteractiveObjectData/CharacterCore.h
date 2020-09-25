// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "CharacterCore.generated.h"

class AInteractiveAbility;

USTRUCT(BlueprintType)
struct WORKSHOP_API FCharacterCore
{
  GENERATED_BODY()

  // --------- //
  // Abilities //
  // --------- //

  // Abilities which Interactive character can use.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterSettings")
  TArray<TSubclassOf<AInteractiveAbility>> AbilitiesClasses;
  UPROPERTY() TArray<AInteractiveAbility*> Abilities;

  // ------- //
  // Visuals //
  // ------- //

  // Map of animations with their integer identifiers.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterSettings")
  TMap<int32, UPaperFlipbook*> AnimationsMap;
};
