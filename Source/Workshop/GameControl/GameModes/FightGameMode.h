// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Workshop/ActorClasses/Managers/TurnBasedManager.h"
#include "FightGameMode.generated.h"

class AFightController;

DECLARE_MULTICAST_DELEGATE(FRegisterSpawns)
DECLARE_MULTICAST_DELEGATE(FObjectsReady)

UCLASS(Blueprintable)
class WORKSHOP_API AFightGameMode : public AGameModeBase
{
  GENERATED_BODY()

public:
  FRegisterSpawns RegisterAllSpawnLocations;
  FObjectsReady ObjectsReady;
  TArray<TPair<int32, AFightController*>> FightControllers;
  ATurnBasedManager* FightManager;

public:
  AFightGameMode();
};
