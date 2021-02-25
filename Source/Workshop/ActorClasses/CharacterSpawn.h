// Copyright © Dmitriy Shepelin 2021. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Managers/TurnBasedManager.h"
#include "Workshop/GameControl/PlayerControllers/InteractController.h"
#include "CharacterSpawn.generated.h"


/**
 * This object can be used by TurnBasedManager to desribe where to put
 * characters on a level.
 */
UCLASS()
class ACharacterSpawn : public AActor
{
  GENERATED_BODY()

protected:
  /** Desribe the direction where the potential character should be headed. */
  UPROPERTY(EditAnywhere) int Direction;
};
