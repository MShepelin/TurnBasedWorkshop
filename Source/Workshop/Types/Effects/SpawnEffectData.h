// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EffectData.h"
#include "SpawnEffectData.generated.h"


UCLASS(Blueprintable, BlueprintType)
class WORKSHOP_API USpawnEffectData : public UEffectData
{
  GENERATED_BODY()

protected:
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings")
  TSubclassOf<AInteractiveObject> ClassToSpawn;

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings", meta = (ClampMin = "0"))
  int32 NumberOfSpawns;

public:
  USpawnEffectData();

  //++++ override GatherInformation function
};