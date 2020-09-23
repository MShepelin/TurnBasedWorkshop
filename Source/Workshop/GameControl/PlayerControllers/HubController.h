// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CameraController.h"
#include "Stats/Stats.h"
#include "Async/AsyncWork.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/UObjectBaseUtility.h"
#include "HubController.generated.h"


class AInteractiveCharacter;


UCLASS(Blueprintable)
class WORKSHOP_API AHubController : public ACameraController
{
	GENERATED_BODY()

private:
  UPROPERTY() TSet<AInteractiveCharacter*> ChosenCharacters;
  UPROPERTY() bool bStartedLevelLoading = false;

  UFUNCTION() void ChooseCharacter();

  UFUNCTION() void ReadyForNextLevel();
public:
  AHubController();

  void SetupInputComponent() override;

  UFUNCTION() void ApplyChosenCharacters();
};
