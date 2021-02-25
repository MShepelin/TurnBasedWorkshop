// Copyright © Dmitriy Shepelin 2021. MIT License.

#include "FightGameMode.h"
#include "Workshop/ActorClasses/CameraWork/SpryCamera.h"
#include "Workshop/GameControl/PlayerControllers/InteractController.h"
#include "../AI/FightAI.h"

AFightGameMode::AFightGameMode()
{
  DefaultPawnClass = ASpryCamera::StaticClass();
  PlayerControllerClass = AInteractController::StaticClass();
}
