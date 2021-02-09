// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RegistrationManager.h"
#include "Workshop/Types/TurnPhase.h"
#include "Workshop/Types/Nonblueprintable/GameConstants.h"
#include "Workshop/Types/SpawnCollection.h"
#include "TurnBasedManager.generated.h"

class UTurnBasedComponent;
class AInteractiveCharacter;
class ATurnBasedObserver;
class ACharacterSpawn;

// Supports turn's phases change for connected controllers, which can join any time.
UCLASS()
class WORKSHOP_API ATurnBasedManager : public ARegistrationManager
{
	GENERATED_BODY()

protected:
  UPROPERTY() TArray<ATurnBasedObserver*> JoinedControllers;

  UPROPERTY() int32 CurrentControllerIndex;

  UPROPERTY() ETurnPhase CurrentTurnPhase = ETurnPhase::Start;

  UPROPERTY(EditAnywhere)
  TMap<int32, FSpawnCollection> SpawnLocations;
  
public:
  ATurnBasedManager();

  UFUNCTION(BlueprintCallable) void NextPhase();

  // Controller will join turn order last.
  UFUNCTION(BlueprintCallable)
  void AddTurnBasedController(ATurnBasedObserver* NewController);

  UFUNCTION(BlueprintCallable)
  void RemoveTurnBasedController(ATurnBasedObserver* NewController);

  UFUNCTION(BlueprintCallable)
  ETurnPhase GetPhase() const;

  UFUNCTION(BlueprintCallable)
  const TArray<ACharacterSpawn*>& GetCharacterSpawns(int32 TurnBasedID) const;
};
