// Copyright © Dmitriy Shepelin 2021. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "SpawnCollection.generated.h"

class ACharacterSpawn;

/** Stores spawn options for one TurnBasedObserver. */
USTRUCT(BlueprintType)
struct WORKSHOP_API FSpawnCollection
{
  GENERATED_BODY()

  /** Locations of characters to use when they are spawned. */
  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  TArray<ACharacterSpawn*> CharacterSpawns;
};
