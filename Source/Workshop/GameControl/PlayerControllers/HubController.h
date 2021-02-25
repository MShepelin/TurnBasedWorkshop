// Copyright © Dmitriy Shepelin 2021. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "CameraController.h"
#include "Stats/Stats.h"
#include "Workshop/Types/InteractiveObjectData/CharacterCore.h"
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
