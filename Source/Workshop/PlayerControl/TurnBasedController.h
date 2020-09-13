// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TurnBasedController.generated.h"

/**
 * 
 */
UCLASS()
class WORKSHOP_API ATurnBasedController : public APlayerController
{
	GENERATED_BODY()
private:
  // Tries to pick an Interactive object
  UFUNCTION() void TryToInteract();

public:
  ATurnBasedController();

  void SetupInputComponent() override;
};
