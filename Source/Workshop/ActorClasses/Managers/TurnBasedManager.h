// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RegistrationManager.h"
#include "Workshop/Types/TurnPhase.h"
#include "TurnBasedManager.generated.h"

// Supports turn's phases change for connected controllers, which can join any time.
UCLASS()
class WORKSHOP_API ATurnBasedManager : public ARegistrationManager
{
	GENERATED_BODY()

protected:
  UPROPERTY() TArray<AController*> JoinedControllers;
  UPROPERTY() AController* CurrentController;
  UPROPERTY() ETurnPhase CurrentTurnPhase = ETurnPhase::Start;
  
public:
  UFUNCTION(BlueprintCallable)
  void JoinLast(AController* NewController);

  UFUNCTION(BlueprintCallable)
  void NextPhase();
};
