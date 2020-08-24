// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "InteractiveCharacter.h"
#include "InteractiveAbility.generated.h"


// Represenatation of character's gameplay possibilities on game scene.
UCLASS(Blueprintable)
class WORKSHOP_API AInteractiveAbility : public AInteractiveObject
{
  GENERATED_BODY()

protected:
  AInteractiveCharacter* CharacterOwner;

  // Animation identifier which should be played by owner when this ability is resolved.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AnimationSettings")
  int32 AbilityAnimationId;

  // This array collectes all effects used in ability.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AnimationSettings")
  TArray<TSubclassOf<UEffectData>> UsedEffects;

  // ------- //
  // Visuals //
  // ------- //

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AnimationSettings", meta = (OverrideNativeName = "AbilityIconInSlot"))
  UPaperFlipbook* Icon;

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AnimationSettings", meta = (OverrideNativeName = "AbilityIconInScene"))
  UPaperFlipbook* IconScene;

public:
  AInteractiveAbility();

  AInteractiveAbility(AInteractiveCharacter* Owner);

  // ----------------- //
  // Ability's actions //
  // ----------------- //

  UFUNCTION(BlueprintNativeEvent)
  void CustomEffect(AInteractiveObject* TargetObject);
  virtual void CustomEffect_Implementation(AInteractiveObject* TargetObject);

  void ResolveAbility();

  // ------ //
  // Others //
  // ------ //

  virtual FString GatherInformation() const override;

  void ShowInfluences() const override;

  friend class UBuildAbility; // for optimisation purposes
};
