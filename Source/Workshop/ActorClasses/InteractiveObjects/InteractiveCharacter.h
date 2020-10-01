// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "Workshop/Types/Effects/EffectData.h"
#include "InteractiveObject.h"
#include "Components/BoxComponent.h"
#include "Workshop/Types/InteractiveObjectData/CharacterCore.h"
#include "InteractiveCharacter.generated.h"

class AInteractiveAbility;
class UAbilitiesWidget;
class UInformationWidget;
class UAbilitySlot;

// Interactive characters have visual represenation, abilities, types, 
// they are able to take actions in turn-based events and connected to 
// skill activations.
UCLASS(Abstract, Blueprintable)
class WORKSHOP_API AInteractiveCharacter : public AInteractiveObject
{
	GENERATED_BODY()

protected:
  //???? add state machine support functions

  UPROPERTY(VisibleDefaultsOnly)
  UPaperFlipbookComponent* CharacterPresentation;

  UPROPERTY() AInteractiveAbility* CentralAbility = nullptr;

  // Used for receiving player's input and set object's world transform.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterSettings")
  UBoxComponent* CollisionBox;

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterSettings")
  FVector2D CentralAbilityRelativePositionInput;

  UPROPERTY() FVector CentralAbilityRelativePosition;

  UPROPERTY()
  UBillboardComponent* CentralAbilityPositionVisual;

public:
  // ---------------------- //
  // Characters Description //
  // ---------------------- //

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
  FCharacterCore CharacterDataCore;

  UPROPERTY() TArray<AInteractiveAbility*> Abilities;

  AInteractiveCharacter();

  // ------------------------- //
  // AActor functions overload //
  // ------------------------- //

  virtual void PostInitProperties() override;

  virtual void BeginPlay() override;

  virtual void OnConstruction(const FTransform & Transform) override;

  virtual void PostEditChangeProperty(struct FPropertyChangedEvent& ChangeEvent);

  // ---------- //
  // Animations //
  // ---------- //

  // Play animation by its ID
  void PlayAnimation(int32 AnimationID);

  // ----------------------- //
  // Connection with Manager //
  // ----------------------- //

  virtual void PickedAsCentral() override;

  virtual void UnpickedAsCentral() override;

  // ----------------------//
  // Character Information //
  // --------------------- //

  //int32 GetProtectionFromMask() const;

  // ----------------- //
  // UI with abilities //
  // ----------------- //

  UFUNCTION(BlueprintCallable)
  void ChangeCentralAbilityVisibility(bool bIsInvisible);

  void SetCentralAbility(AInteractiveAbility* Ability);

  // ------ //
  // Others //
  // ------ //

  void RefreshInteractive() override;

  friend class UBuildAbility; // for optimisation purposes
};
