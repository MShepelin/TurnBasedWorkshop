// Fill out your copyright notice in the Description page of Project Settings.

#include "FightGameMode.h"
#include "Workshop/ActorClasses/CameraWork/SpryCamera.h"
#include "Workshop/GameControl/PlayerControllers/InteractController.h"
#include "Workshop/UI/TurnBasedEvent/TurnBasedHUD.h"

AFightGameMode::AFightGameMode()
{
  DefaultPawnClass = ASpryCamera::StaticClass();
  PlayerControllerClass = AInteractController::StaticClass();
  HUDClass = ATurnBasedHUD::StaticClass();
}
