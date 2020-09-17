// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "Templates/SharedPointer.h"
#include "InteractiveCharacter.h"
#include "InteractiveAbility.generated.h"


class AInteractiveCharacter;


// Represenatation of character's gameplay possibilities on game scene.
UCLASS(Blueprintable)
class WORKSHOP_API AInteractiveAbility : public AInteractiveObject
{
  GENERATED_BODY()

protected:
  // -------------------------- //
  // Ability gameplay specifics //
  // -------------------------- //

  AInteractiveCharacter* CharacterOwner;

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
  //++++ add num limit for ability to add influenced objects

  // ------- //
  // Visuals //
  // ------- //

  // Icon of Ability in UI.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AnimationSettings")
  UTexture2D* IconUI;

  // Icon of Ability on scene.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AnimationSettings")
  UPaperFlipbook* IconScene;

  UPROPERTY(VisibleDefaultsOnly)
  UPaperFlipbookComponent* AbilityPresentation;

public:
  AInteractiveAbility();

  // Used to initialise character-owner.
  void SetCharacterOwner(AInteractiveCharacter* NewCharacterOwner);

  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

  virtual void PostInitProperties() override;

  virtual void BeginPlay() override;

  // ----------------------- //
  // Connection with Manager //
  // ----------------------- //

  virtual void PickedAsCentral() override;

  virtual void UnpickedAsCentral() override;

  virtual void PickedAsTarget() override;

  virtual void UnpickedAsTarget() override;

  // ----------------- //
  // Ability's actions //
  // ----------------- //

  UFUNCTION(BlueprintNativeEvent)
  void CustomEffect(AInteractiveObject* TargetObject);
  virtual void CustomEffect_Implementation(AInteractiveObject* TargetObject);

  // Uses CustomEffect on every Influenced Object
  UFUNCTION(BlueprintCallable)
  void ResolveAbility();

  void CenterInOwner();

  // ------------------ //
  // Turn-based actions //
  // ------------------ //

  virtual void SetTurn(ETurnPhase TurnPhase) override;

  // ------------------- //
  // Ability's variables //
  // ------------------- //

  int32 GetTargetTypeMask() const;

  UTexture2D* GetIconUI() const;

  // ------ //
  // Others //
  // ------ //

  // REMAKE
  // virtual FString GatherInformation() const override;

  void ShowInfluences() const override;

  friend class UBuildAbility; // for optimisation purposes
};
