// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Workshop/ActorClasses/CameraWork/SpryCamera.h"
#include "Workshop/Types/Components/TurnBasedComponent.h"
#include "ChoicesInstance.h"
#include "InteractController.generated.h"


class ATurnBasedManager;
class AInteractiveCharacter;


UCLASS(Blueprintable)
class WORKSHOP_API AInteractController : public APlayerController
{
	GENERATED_BODY()

private:
  // UPROPERTY() TScriptInterface<ITurnBasedCamera> CurrentCamera = nullptr;
  UPROPERTY() ASpryCamera* CurrentCamera;

protected:
  UPROPERTY(VisibleDefaultsOnly) UTurnBasedComponent* TurnControl;
  UPROPERTY() ATurnBasedManager* EventManager = nullptr;

  UPROPERTY() TArray<AInteractiveCharacter*> PlacableCharacters;

  // Tries to pick an Interactive object
  UFUNCTION() void StartInteract();
  UFUNCTION() void StopInteract();

  UFUNCTION() void ConnectionHappened();

public:
  AInteractController();

  void BeginPlay() override;

  void SetupInputComponent() override;

  virtual void SetPawn(APawn * InPawn) override;

  // REMAKE
  // void ConnectToEvent(ARegistrationManager* NewManager);

  ASpryCamera* GetCurrentCamera() const;
};

//???? may be make a basic class, which considers only 
//     TurnBasedController and isn't connected with InteractiveObjects of any kind
