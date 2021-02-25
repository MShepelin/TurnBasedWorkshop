// Copyright © Dmitriy Shepelin 2021. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "../Effects/EffectData.h"
#include "../InteractiveType.h"
#include "PaperFlipbookComponent.h"
#include "Containers/Queue.h"
#include "AbilityCore.generated.h"

USTRUCT(BlueprintType)
struct WORKSHOP_API FAbilityCore
{
  GENERATED_BODY()

  // -------------------------- //
  // Ability gameplay specifics //
  // -------------------------- //

  // Animation identifier which should be played by owner when this ability is resolved.
  UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AbilitySettings")
  int32 AbilityAnimationId;

  // This array collectes all effects used in ability.
  UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AbilitySettings")
  TArray<FEffectData> UsedEffects;

  TQueue<FEffectData> EffectsToResolve;
  TQueue<FEffectData> EffectsToReceive;

  UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AbilitySettings")
  TArray<int32> CTsToAffect;

  // Target on which this affect is applicable.
  UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AbilitySettings", meta = (Bitmask, BitmaskEnum = "EInteractiveType"))
  int32 TargetTypeMask = static_cast<int32>(EInteractiveType::Character);

  // Number of objects which can be chosen.
  UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AbilitySettings", meta = (ClampMin = "1"))
  int32 NumOfTargets = 1;

  // ------- //
  // Visuals //
  // ------- //

  // Icon of Ability in UI.
  UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AnimationSettings")
  UTexture2D* IconUI;

  // Icon of Ability on scene.
  UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AnimationSettings")
  UPaperFlipbook* IconScene;

  FAbilityCore& operator=(const FAbilityCore& AnotherCore);
};
