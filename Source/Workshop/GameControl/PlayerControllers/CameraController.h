// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Workshop/ActorClasses/CameraWork/SpryCamera.h"
#include "Workshop/ActorClasses/Managers/RegistrationManager.h"
#include "CameraController.generated.h"

UCLASS(Blueprintable)
class WORKSHOP_API ACameraController : public APlayerController
{
  GENERATED_BODY()

protected:
  UPROPERTY() ASpryCamera* CurrentCamera;
  UPROPERTY() ARegistrationManager* UsedManager = nullptr;

  UFUNCTION() AActor* GeneralRayCast();

public:
  ACameraController();

  void BeginPlay() override;

  void SetPawn(APawn * InPawn) override;

  ASpryCamera* GetCurrentCamera() const;

  const ARegistrationManager* GetUsedManager() const;
};
