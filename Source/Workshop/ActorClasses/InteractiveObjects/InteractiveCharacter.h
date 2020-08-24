// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "Workshop/Types/Effects/EffectData.h"
#include "InteractiveCharacter.generated.h"


class AInteractiveAbility;

// Interactive characters have visual represenation, abilities, types, 
// they are able to take actions in turn-based events and connected to 
// skill activations.
UCLASS(Blueprintable)
class WORKSHOP_API AInteractiveCharacter : public AInteractiveObject
{
	GENERATED_BODY()

protected:
  // Map of animations with their integer identifiers.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
  TMap<int32, UPaperFlipbook*> AnimationsMap;

  //???? add state machine support functions

  UPaperFlipbookComponent* CharacterPresentation;

  // Abilities which Interactive character can use.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterSettings | AbilitiesControl")
  TArray<TSubclassOf<AInteractiveAbility>> Abilities;

private:
  void RemoveEffectByIndex(int32 EffectIndex);

public:
  AInteractiveCharacter();

  // ------------------------ //
  // Actor functions overload //
  // ------------------------ //

  virtual void PostInitProperties() override;

  // ---------- //
  // Animations //
  // ---------- //

  // Play animation by its ID
  void PlayAnimation(int32 AnimationID);

  // ----------------------- //
  // Applied effects changes //
  // ----------------------- //

  // Effect would be removed if any flag of mask is met.
  void RemoveEffectsBySpecifiersMask(int32 mask);

  // ---------- //
  // Turn-based //
  // ---------- //

  void OnTurnStart();

  void OnTurnEnd();

  // ------ //
  // Others //
  // ------ //

  virtual FString GatherInformation() const override;

  void ShowInfluences() const override;

  friend class UBuildAbility; // for optimisation purposes
};
