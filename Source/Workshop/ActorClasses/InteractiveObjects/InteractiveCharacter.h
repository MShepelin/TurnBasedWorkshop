// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "Workshop/Types/Effects/EffectData.h"
#include "InteractiveCharacter.generated.h"


class AInteractiveAbility;

/**
 * Interactive characters have visual represenation, abilities, types, 
 * they are able to take actions in turn-based events and connected to 
 * skill activations
 */
UCLASS(Blueprintable)
class WORKSHOP_API AInteractiveCharacter : public AInteractiveObject
{
	GENERATED_BODY()

protected:
  // Animation state machine (may be connected??? graph???)
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
  TMap<int32, UPaperFlipbook*> AnimationsCollection;
  UPaperFlipbookComponent* CharacterPresentation;

  // Character statisctics in form of strings
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterSettings | Statistics")
  TMap<int32, FString> StringStats = 
  { 
    {CharacterNameStatId, DefaultStringValue},
  };

  // Character statisctics in form of integers
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterSettings | Statistics")
  TMap<int32, int32> IntegerStats =
  {
    {HealthStatId, DefaultPointsValue},
  };

  // Statistics Principals:
  // every stat has it's id, which stays the same for the whole game
  //++++ id description should be gathered in some document
  //++++ also editor should be modified in a way to show this desprions for developer

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterSettings | AbilitiesControl")
  TArray<TSubclassOf<AInteractiveAbility>> Abilities;

  // List of effects which are applied to the character in the current state
  TArray<UEffectData*> AccumulatedEffects;

  friend class UBuildAbility;


  /**
   * @warning: Doesn't make sanity checks!
   */
  void RemoveEffectByIndex(int32 EffectIndex);

public:
  AInteractiveCharacter();

  virtual void PostInitProperties() override;

  void PlayAnimation(int32 AnimationId);

  /**
   * Effect would be removed if any flag of mask is met.
   */
  void RemoveEffectsBySpecifiersMask(int32 mask);

  virtual void GatherInformation() const override;

  void ShowInfluences() const override;

  void OnTurnStart();

  void OnTurnEnd();
};
