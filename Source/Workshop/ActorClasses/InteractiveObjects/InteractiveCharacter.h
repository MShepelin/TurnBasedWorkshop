// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "Workshop/Types/Effects/EffectData.h"
#include "InteractiveCharacter.generated.h"

const int32    DefaultPointsValue   = 10;
const FString  DefaultStringValue   = "Dude";

const int32    CharacterNameStatId  = 0;
const int32    HealthStatId         = 1;

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

  // Character Statistics
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
  TMap<int32, FString> StringStats = 
  { 
    {CharacterNameStatId, DefaultStringValue},
  };

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
  TMap<int32, int32> IntegerStats =
  {
    {HealthStatId, DefaultPointsValue},
  };

  // Statistics Principals:
  // every stat has it's id, which stays the same for the whole game
  //++++ id description should be gathered in some document
  //++++ also editor should be modified in a way to show this desprions for developer

  // List of effects which are applied to the character in the current state
  TArray<UEffectData*> AccumulatedEffects;

public:
  // ---------------------------------------------------------------
  //                        Construction
  // ---------------------------------------------------------------

  AInteractiveCharacter();

  virtual void OnConstruction(const FTransform & Transform) override;

  void PlayAnimation(int32 AnimationId);

  // ---------------------------------------------------------------
  //            Change effects, witch Character already has
  // ---------------------------------------------------------------

  void RemoveEffectsBySpecifiersMask(int mask);

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

  void OnTurnStart();

  void OnTurnEnd();
};
