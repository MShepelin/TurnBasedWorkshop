// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Controller.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveCharacter.h"
#include "Workshop/Types/Components/TurnBasedComponent.h"
#include "FightController.generated.h"

UCLASS(Blueprintable)
class WORKSHOP_API AFightController : public AController
{
  GENERATED_BODY()

public:
  // Unique for every controller (0 - means player controller).
  UPROPERTY() int32 ControllerID;

  UPROPERTY() TArray<AInteractiveCharacter*> PlacableCharacters;
  TArray<TPair<int32, FTransform>> CharactersSpawnTransforms;

  UPROPERTY(VisibleDefaultsOnly) UTurnBasedComponent* TurnControl;

public:
  AFightController();

  void ConnectionHappened();
};
