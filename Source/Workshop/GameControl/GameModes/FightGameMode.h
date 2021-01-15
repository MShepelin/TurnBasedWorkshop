// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Workshop/ActorClasses/Managers/TurnBasedManager.h"
#include "Workshop/WorkshopGameModeBase.h"
#include "FightGameMode.generated.h"

class AFightAI;

DECLARE_MULTICAST_DELEGATE(FRegisterSpawns)
DECLARE_MULTICAST_DELEGATE(FObjectsReady)

UCLASS(Blueprintable)
class WORKSHOP_API AFightGameMode : public AWorkshopGameModeBase
{
  GENERATED_BODY()

public:
  FRegisterSpawns RegisterAllSpawnLocations;
  FObjectsReady ObjectsReady;
  TArray<TPair<int32, AFightAI*>> FightControllers;

  UPROPERTY(BlueprintReadOnly)
  ATurnBasedManager* FightManager;

public:
  AFightGameMode();
};
