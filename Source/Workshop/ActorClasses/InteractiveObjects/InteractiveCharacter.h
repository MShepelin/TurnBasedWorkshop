// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveObject.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "InteractiveCharacter.generated.h"

class UEffectData;

// Interactive characters have visual represenation, abilities, types, 
// they are able to take actions in turn-based events and connected to 
// skill activations
UCLASS(Blueprintable)
class WORKSHOP_API AInteractiveCharacter : public AInteractiveObject
{
	GENERATED_BODY()

protected:
  // Animation state machine (may be connected??? graph???)
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
  TMap<int32, UPaperFlipbook*> AnimationsCollection;
  UPaperFlipbookComponent* CharacterPresentation;

  // Stats

  // Effects
  TArray<UEffectData*> AppliedEffects;

public:
  // ---------------------------------------------------------------
  //                              Setup
  // ---------------------------------------------------------------

  AInteractiveCharacter();

  virtual void OnConstruction(const FTransform & Transform) override;

  void PlayAnimation(int32 AnimationId);

  // ---------------------------------------------------------------
  //         Step, when player can interact with character
  // ---------------------------------------------------------------

  virtual void GatherInformation() override;

  void ShowInfluences() override;

  // ---------------------------------------------------------------
  //              Step, when all actions are resolved
  // ---------------------------------------------------------------

  // ???

  //----------------------------------------------------------------
  //                  Character's turn
  // ---------------------------------------------------------------

  void ResolveEffects();

  void OnTurnStart();

};
