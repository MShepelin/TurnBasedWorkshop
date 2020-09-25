// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Effects/EffectData.h"
#include "PaperFlipbookComponent.h"
#include "AbilityCore.generated.h"

USTRUCT(BlueprintType)
struct WORKSHOP_API FAbilityCore
{
  GENERATED_BODY()

  // -------------------------- //
  // Ability gameplay specifics //
  // -------------------------- //

  // Animation identifier which should be played by owner when this ability is resolved.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AbilitySettings")
  int32 AbilityAnimationId;

  // This array collectes all effects used in ability.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Category = "AbilitySettings")
  TArray<UEffectData*> UsedEffects;

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AbilitySettings")
  TArray<int32> CTsToAffect;

  // Target on which this affect is applicable.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AbilitySettings", meta = (Bitmask, BitmaskEnum = "EInteractiveType"))
  int32 TargetTypeMask = static_cast<int32>(EInteractiveType::Character);

  // Number of objects which can be chosen.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AbilitySettings", meta = (ClampMin = "1"))
  int32 NumOfTargets = 1;

  // ------- //
  // Visuals //
  // ------- //

  // Icon of Ability in UI.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AnimationSettings")
  UTexture2D* IconUI;

  // Icon of Ability on scene.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AnimationSettings")
  UPaperFlipbook* IconScene;
};
