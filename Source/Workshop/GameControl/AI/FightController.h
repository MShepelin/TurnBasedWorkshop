// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Controller.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveCharacter.h"
#include "Workshop/ActorClasses/Managers/TurnBasedManager.h"
#include "Workshop/Types/Components/TurnBasedComponent.h"
#include "FightController.generated.h"

UCLASS(Blueprintable)
class WORKSHOP_API AFightController : public AController
{
  GENERATED_BODY()

public:
  UPROPERTY() ATurnBasedManager* UsedManager;

  // Unique for every controller. Used to sort controllers and give them CharacterSpawns.
  UPROPERTY() int32 ControllerID;

  UPROPERTY(EditInstanceOnly) TArray<TSubclassOf<AInteractiveCharacter>> CharacterClasses;
  UPROPERTY() TArray<AInteractiveCharacter*> SpawnedCharacters;
  TArray<TPair<int32, FTransform>> CharactersSpawnTransforms;

  UPROPERTY(VisibleDefaultsOnly) UTurnBasedComponent* TurnControl;

public:
  AFightController();

  void PostInitializeComponents() override;

  UFUNCTION() void ConnectionHappened();

  UFUNCTION() void TurnControllGained();
};
