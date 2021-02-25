// Copyright © Dmitriy Shepelin 2021. MIT License.

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
