// Copyright © Dmitriy Shepelin 2021. MIT License.

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

/**
 * Interactive characters have Paper2D visual represenation, abilities and animations.
 * They also implement the concept of central ability, which will be used if a character 
 * gets an opportunity to act.
 */
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

  virtual void BeginPlay() override;

  virtual void OnConstruction(const FTransform & Transform) override;

  virtual void Tick(float DeltaTime) override;

  // ---------- //
  // Animations //
  // ---------- //

  /**
   * Play animation by it's ID thread-safely.
   * Returns true if ID was found, and false if not.
   * If bWaitUntilEnds is true the thread that called this function will sleep until the animation is finished.
   */
  virtual void PlayAnimation(int32 AnimationId, bool bWaitUntilEnds) override;

  /** Return to default animation thread-safely. */
  UFUNCTION(BlueprintCallable)
  void ResetAnimation();

  /** Any additional actions that should happen when animation starts to play with given duration and ID. */
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

  /** If a creature isn't exhausted this function activates central ability as an action of this character. */
  UFUNCTION(BlueprintCallable)
  void ResolveCharacterActions();

  /** Checks stats and apply conditions based on their values. */
  void UpdateExhaust() override;

  /** 
   * Before characterc can their central abilities must have sertain preparations. 
   * In other words, this function should be called one time before ResolveCharacterActions().
   */
  void PrepareCentralAbilityToResolve();

  // ----------------- //
  // UI with abilities //
  // ----------------- //

  /** Change visibility of the ability that is currently central for this character. */
  UFUNCTION(BlueprintCallable)
  void ChangeCentralAbilityVisibility(bool bIsInvisible);

  /** Make some ability central and move it to an appropriate location. */
  UFUNCTION(BlueprintCallable)
  void SetCentralAbility(AInteractiveAbility* Ability);

  /** 
   * Hide central ability visually. 
   * If bOnlyVisually is false remove all influences and dependecies of the current central ability
   * and mark that this character doesn't have central ability anymore
   */
  UFUNCTION(BlueprintCallable)
  void ClearCentralAbility(bool bOnlyVisually = false);

  // ------ //
  // Others //
  // ------ //

  void BuildInteractive() override;

  friend class UBuildAbility; // for optimisation purposes
};
