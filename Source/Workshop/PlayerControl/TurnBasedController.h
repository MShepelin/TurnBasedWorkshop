// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Workshop/ActorClasses/CameraWork/SpryCamera.h"
#include "TurnBasedController.generated.h"


class ARegistrationManager;


UCLASS(Blueprintable)
class WORKSHOP_API ATurnBasedController : public APlayerController
{
	GENERATED_BODY()

private:
  // UPROPERTY() TScriptInterface<ITurnBasedCamera> CurrentCamera = nullptr;
  UPROPERTY() ASpryCamera* CurrentCamera;

protected:
  UPROPERTY() FVector StoreLaction = FVector(0, 0, 0);

  UPROPERTY() ARegistrationManager* EventManager;

  // Tries to pick an Interactive object
  UFUNCTION() void TryToInteract();
  UFUNCTION() void StopInteract();

public:
  ATurnBasedController();

  void BeginPlay() override;

  void SetupInputComponent() override;

  virtual void SetPawn(APawn * InPawn) override;

  // REMAKE
  //void ConnectToEvent(ARegistrationManager* NewManager);

  ASpryCamera* GetCurrentCamera() const;
};
