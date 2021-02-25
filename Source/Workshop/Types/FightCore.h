// Copyright © Dmitriy Shepelin 2021. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveCharacter.h"
#include "FightCore.generated.h"

USTRUCT(BlueprintType)
struct WORKSHOP_API FFightCore
{
  GENERATED_BODY()

  UPROPERTY(EditDefaultsOnly)
  TArray<TSubclassOf<AInteractiveCharacter>> FightEnemies;

  UPROPERTY(EditDefaultsOnly)
  TSubclassOf<AController> FightAI;
};
