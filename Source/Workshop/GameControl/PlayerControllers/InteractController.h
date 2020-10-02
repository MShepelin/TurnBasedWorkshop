// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CameraController.h"
#include "Workshop/Types/Components/TurnBasedComponent.h"
#include "InteractController.generated.h"

class ATurnBasedManager;
class AInteractiveCharacter;

UCLASS(Blueprintable)
class WORKSHOP_API AInteractController : public ACameraController
{
	GENERATED_BODY()

protected:
  UPROPERTY(VisibleDefaultsOnly) UTurnBasedComponent* TurnControl;
  UPROPERTY() ATurnBasedManager* EventManager = nullptr;

  UPROPERTY() TArray<AInteractiveCharacter*> PlacableCharacters;

  AInteractiveCharacter* FirstToSwap[2];
  UPROPERTY() bool bSwapModeIsActive;

  // Tries to pick an Interactive object
  UFUNCTION() void StartInteract();
  UFUNCTION() void StopInteract();

  UFUNCTION() void ConnectionHappened();
  
  UFUNCTION() void SetSwapMode(bool bIsActive);

public:
  AInteractController();

  void BeginPlay() override;

  void SetupInputComponent() override;

  UFUNCTION(BlueprintCallable)
  void ResolveCharactersAbilities();
};
