// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Workshop/ActorClasses/Managers/TurnBasedManager.h"
#include "FightGameMode.generated.h"

DECLARE_MULTICAST_DELEGATE(FRegisterSpawns)

UCLASS(Blueprintable)
class WORKSHOP_API AFightGameMode : public AGameModeBase
{
  GENERATED_BODY()

public:
  FRegisterSpawns RegisterAllSpawnLocations;
  ATurnBasedManager* FightManager;

public:
  AFightGameMode();
};
