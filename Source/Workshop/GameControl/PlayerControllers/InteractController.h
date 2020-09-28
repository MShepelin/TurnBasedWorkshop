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

  UPROPERTY(EditDefaultsOnly) TSubclassOf<AInteractiveCharacter> SpawnCharacterClass = nullptr;

  // Tries to pick an Interactive object
  UFUNCTION() void StartInteract();
  UFUNCTION() void StopInteract();

  UFUNCTION() void ConnectionHappened();

public:
  AInteractController();

  void BeginPlay() override;

  void SetupInputComponent() override;
};
