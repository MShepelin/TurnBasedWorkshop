// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "InteractiveCharacter.h"
#include "InteractiveAbility.generated.h"


class UEffectData;


/**
 * Visual representation of Ability, which holds all needed information about its' effects.
 * Can be connected to other Interactive Objects to apply effects on them.
 */
UCLASS(Blueprintable)
class WORKSHOP_API AInteractiveAbility : public AInteractiveObject
{
  GENERATED_BODY()

protected:
  EInteractiveType InteractiveType = EInteractiveType::Ability;

  AInteractiveCharacter* CharacterOwner;

  // Animation identifier which should be played by owner, when ability is resolved
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AnimationSettings")
  int32 AbilityAnimationId;

  // Used to accumulate effects for this ability
  TArray<UEffectData> UsedEffects;

public:
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AnimationSettings", meta = (OverrideNativeName = "AbilityIconInSlot"))
  UPaperFlipbook* Icon;

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AnimationSettings", meta = (OverrideNativeName = "AbilityIconInScene"))
  UPaperFlipbook* IconScene;

  AInteractiveAbility();

  AInteractiveAbility(AInteractiveCharacter* Owner);

  UFUNCTION(BlueprintNativeEvent)
  void CustomEffect(AInteractiveObject* aim);
  virtual void CustomEffect_Implementation(AInteractiveObject* aim);

  virtual void GatherInformation() const override;

  void ShowInfluences() const override;

  void ResolveAbility();
};
