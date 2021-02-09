//

#pragma once

#include "CoreMinimal.h"
#include "SpawnCollection.generated.h"

class ACharacterSpawn;

USTRUCT(BlueprintType)
struct WORKSHOP_API FSpawnCollection
{
  GENERATED_BODY()

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  TArray<ACharacterSpawn*> CharacterSpawns;
};
