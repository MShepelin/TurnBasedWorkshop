// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "Templates/SharedPointer.h"
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
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AbilitySettings")
  int32 AbilityAnimationId;

  // This array collectes all effects used in ability.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Category = "AbilitySettings")
  TArray<UEffectData*> UsedEffects;

  // ------- //
  // Visuals //
  // ------- //

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AnimationSettings", meta = (OverrideNativeName = "AbilityIconInSlot"))
  UPaperFlipbook* IconUI;

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AnimationSettings", meta = (OverrideNativeName = "AbilityIconOnScene"))
  UPaperFlipbook* IconScene;

public:
  AInteractiveAbility();

  AInteractiveAbility(AInteractiveCharacter* Owner);

  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

  virtual void PostInitProperties() override;

  virtual void BeginPlay() override;

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
