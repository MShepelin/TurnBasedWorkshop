// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Workshop/WorkshopGameModeBase.h"
#include "HubGameMode.generated.h"

UCLASS()
class WORKSHOP_API AHubGameMode : public AWorkshopGameModeBase
{
  GENERATED_BODY()

public:
  AHubGameMode();
};
