// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CameraController.h"
#include "HubController.generated.h"


class AInteractiveCharacter;


UCLASS(Blueprintable)
class WORKSHOP_API AHubController : public ACameraController
{
	GENERATED_BODY()

private:
  UFUNCTION() void ChooseCharacter();

  UPROPERTY() TSet<AInteractiveCharacter*> ChosenCharacters;

public:
  AHubController();

  void SetupInputComponent() override;

  void ApplyChosenCharacters();
};
