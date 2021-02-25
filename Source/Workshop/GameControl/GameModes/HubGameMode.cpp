// Copyright © Dmitriy Shepelin 2021. MIT License.

#include "HubGameMode.h"
#include "Workshop/ActorClasses/CameraWork/SpryCamera.h"
#include "Workshop/GameControl/PlayerControllers/HubController.h"

AHubGameMode::AHubGameMode()
{
  DefaultPawnClass = ASpryCamera::StaticClass();
  PlayerControllerClass = AHubController::StaticClass();
  //bUseSeamlessTravel = true;
}
