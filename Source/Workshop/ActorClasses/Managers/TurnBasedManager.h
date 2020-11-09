// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RegistrationManager.h"
#include "Workshop/Types/TurnPhase.h"
#include "TurnBasedManager.generated.h"

class UTurnBasedComponent;
class AInteractiveCharacter;

// Supports turn's phases change for connected controllers, which can join any time.
UCLASS()
class WORKSHOP_API ATurnBasedManager : public ARegistrationManager
{
	GENERATED_BODY()

protected:
  UPROPERTY() TArray<UTurnBasedComponent*> JoinedControllers;
  UPROPERTY() int32 CurrentControllerIndex;
  UPROPERTY() int32 AddedControllers;
  UPROPERTY() ETurnPhase CurrentTurnPhase = ETurnPhase::Start;

public:
  // -- //
  // AI //
  // -- //
  TArray<TPair<int32, FTransform>> EnemySpawnLocations;
  UPROPERTY(EditInstanceOnly) TArray<TSubclassOf<AInteractiveCharacter>> EnemyClasses;
  UPROPERTY() TArray<AInteractiveCharacter*> Enemies;
  
public:
  ATurnBasedManager();

  FString PhaseNameBuffer = "";

  // Expected to be called with by TurnBasedComponent, but can be called by other entities.
  UFUNCTION(BlueprintCallable) void NextPhase();

  // Controller will join turn order last.
  UFUNCTION(BlueprintCallable)
  void AddController(AController* NewController);

  UFUNCTION(BlueprintCallable)
  void RemoveController(AController* NewController);

  UFUNCTION(BlueprintCallable)
  void SpawnCharacters();

  UFUNCTION(BlueprintCallable)
  ETurnPhase GetPhase() const;
};
