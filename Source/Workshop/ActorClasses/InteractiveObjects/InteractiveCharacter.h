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
  // Animation state machine
  // +++add state machine support functions
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

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterSettings | AbilitiesControl")
  TArray<TSubclassOf<AInteractiveAbility>> Abilities;

  friend class UBuildAbility;


  // @warning: Doesn't make sanity checks!
  void RemoveEffectByIndex(int32 EffectIndex);

public:
  AInteractiveCharacter();

  virtual void PostInitProperties() override;

  void PlayAnimation(int32 AnimationId);

  //Effect would be removed if any flag of mask is met.
  void RemoveEffectsBySpecifiersMask(int32 mask);

  virtual FString GatherInformation() const override;

  void ShowInfluences() const override;

  void OnTurnStart();

  void OnTurnEnd();
};
