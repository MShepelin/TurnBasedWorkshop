// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveCharacter.h"
#include "InteractiveAbility.generated.h"

class UEffectData;

// Work pipeline: 
// create EffectData variables to have all data needed to resolve effects
// override CustomEffect to use this variables to affect InteractiveObject

//++++ description
UCLASS(Blueprintable)
class WORKSHOP_API AInteractiveAbility : public AInteractiveObject
{
  GENERATED_BODY()

protected:
  AInteractiveCharacter* CharacterOwner;

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
  int32 AbilityAnimationId;

  // Effects specifiers, modified only by CustomEffect
  TArray<TSubclassOf<UEffectData>> UsedEffects;

public:
  // ---------------------------------------------------------------
  //                              Setup
  // ---------------------------------------------------------------

  AInteractiveAbility();

  AInteractiveAbility(AInteractiveCharacter* Owner);

  UFUNCTION(BlueprintNativeEvent)
  void CustomEffect(AInteractiveObject* aim);
  virtual void CustomEffect_Implementation(AInteractiveObject* aim);

  // ---------------------------------------------------------------
  //         Step, when player can interact with character
  // ---------------------------------------------------------------

  virtual void GatherInformation() override;

  void ShowInfluences() override;

  // ---------------------------------------------------------------
  //        Step, when ability must affect interactive object
  // ---------------------------------------------------------------

  void ResolveAbility();
  
};
