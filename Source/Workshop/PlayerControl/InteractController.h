// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Workshop/ActorClasses/CameraWork/SpryCamera.h"
#include "InteractController.generated.h"


class ARegistrationManager;


UCLASS(Blueprintable)
class WORKSHOP_API AInteractController : public APlayerController
{
	GENERATED_BODY()

private:
  // UPROPERTY() TScriptInterface<ITurnBasedCamera> CurrentCamera = nullptr;
  UPROPERTY() ASpryCamera* CurrentCamera;

protected:
  UPROPERTY() FVector StoreLocation = FVector(0, 0, 0);

  UPROPERTY() ARegistrationManager* EventManager;

  // Tries to pick an Interactive object
  UFUNCTION() void StartInteract();
  UFUNCTION() void StopInteract();

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
