// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "Workshop/Types/Effects/EffectData.h"
#include "InteractiveObject.h"
#include "Components/BoxComponent.h"
#include "Workshop/Types/InteractiveObjectData/CharacterCore.h"
#include "Workshop/Types/Nonblueprintable/GameConstants.h"
#include "Components/TimelineComponent.h"
#include "Components/SplineComponent.h"
#include "InteractiveCharacter.generated.h"

class AInteractiveAbility;
class UAbilitiesWidget;
class UInformationWidget;
class UAbilitySlot;
class AInteractController;

// Interactive characters have visual represenation, abilities, types, 
// they are able to take actions in turn-based events and connected to 
// skill activations.
UCLASS(Abstract, Blueprintable)
class WORKSHOP_API AInteractiveCharacter : public AInteractiveObject
{
	GENERATED_BODY()
  
protected:
  UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
  UPaperFlipbookComponent* CharacterPresentation;

  UPROPERTY() AInteractiveAbility* CentralAbility = nullptr;

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
  FVector2D CentralAbilityRelativePositionInput;

  UPROPERTY() FVector CentralAbilityRelativePosition;

  UPROPERTY()
  UBillboardComponent* CentralAbilityPositionVisual;

public:
  AInteractiveCharacter();

  // ---------------------- //
  // Characters Description //
  // ---------------------- //

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
  FCharacterCore CharacterDataCore;

  UPROPERTY() TArray<AInteractiveAbility*> Abilities;

  UFUNCTION(BlueprintCallable)
  AInteractiveAbility* GetCentralAbility() const;

  // ------------------------- //
  // AActor functions overload //
  // ------------------------- //

  virtual void PostInitProperties() override;

  virtual void BeginPlay() override;

  virtual void OnConstruction(const FTransform & Transform) override;

  virtual void Tick(float DeltaTime) override;

  // ---------- //
  // Animations //
  // ---------- //

  // Play animation by it's ID thread-safely.
  // Returns true if ID was found, and false if not.
  virtual void PlayAnimation(int32 AnimationId, bool bWaitUntilEnds) override;

  // Return to playing default animation.
  UFUNCTION(BlueprintCallable)
  void ResetAnimation();

  UFUNCTION(BlueprintNativeEvent)
  void ActionWithAnimation(float Duration, int32 AnimationID);
  virtual void ActionWithAnimation_Implementation(float Duration, int32 AnimationID);

  // ----------------------- //
  // Connection with Manager //
  // ----------------------- //

  virtual void PickedAsCentral() override;

  virtual void UnpickedAsCentral() override;

  void Pick() override;

  /**
    * When a character is connected to a manager this character's abilities need to be connected to.
    * This function is the way to get abilities fast (by pointer).
    */
  TArray<AInteractiveAbility*>* GetAbilties();

  // ------------------ //
  // Turn-Based actions //
  // ------------------ //

  UFUNCTION(BlueprintCallable)
  void ResolveCharacterActions();

  // Checks stats and apply conditions based on their values.
  void UpdateExhaust() override;

  void PrepareCentralAbilityToResolve();

  // ----------------- //
  // UI with abilities //
  // ----------------- //

  UFUNCTION(BlueprintCallable)
  void ChangeCentralAbilityVisibility(bool bIsInvisible);

  UFUNCTION(BlueprintCallable)
  void SetCentralAbility(AInteractiveAbility* Ability);

  UFUNCTION(BlueprintCallable)
  void ClearCentralAbility(bool bOnlyVisually = false);

  // ------ //
  // Others //
  // ------ //

  void BuildInteractive() override;

  friend class UBuildAbility; // for optimisation purposes
};
