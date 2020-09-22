// Fill out your copyright notice in the Description page of Project Settings.

#include "HubGameMode.h"
#include "Workshop/ActorClasses/CameraWork/SpryCamera.h"
#include "Workshop/GameControl/PlayerControllers/HubController.h"
#include "Workshop/UI/TurnBasedEvent/TurnBasedHUD.h"

AHubGameMode::AHubGameMode()
{
  DefaultPawnClass = ASpryCamera::StaticClass();
  PlayerControllerClass = AHubController::StaticClass();
  HUDClass = ATurnBasedHUD::StaticClass();
}
