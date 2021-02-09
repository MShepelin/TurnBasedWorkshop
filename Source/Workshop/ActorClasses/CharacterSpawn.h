//...

#pragma once

#include "CoreMinimal.h"
#include "Managers/TurnBasedManager.h"
#include "Workshop/GameControl/PlayerControllers/InteractController.h"
#include "CharacterSpawn.generated.h"

UCLASS()
class ACharacterSpawn : public AActor
{
  GENERATED_BODY()

public:
  // IDs will be sorted in the ascending order to arrange locations to spawn characters
  //UPROPERTY(EditAnywhere) int32 CharacterOrderID = 0;

  // Controller to which transform should be added.
  // -1 is always PlayerController (as AInteractController).
  //UPROPERTY(EditAnywhere) int32 ControllerID = -1;

public:
  //void PostInitializeComponents() override;

  //void RegisterSpawnTransform();
};
