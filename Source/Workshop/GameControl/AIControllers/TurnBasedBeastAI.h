// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Workshop/ActorClasses/Managers/RegistrationManager.h"
#include "Workshop/Types/Components/TurnBasedComponent.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveCharacter.h"
#include "TurnBasedBeastAI.generated.h"

UCLASS(Blueprintable)
class WORKSHOP_API ATurnBasedBeastAI : public AController
{
  GENERATED_BODY()

protected:
  UPROPERTY() ARegistrationManager* UsedManager = nullptr;

  UPROPERTY(VisibleDefaultsOnly) UTurnBasedComponent* TurnControl;

  UPROPERTY(EditDefaultsOnly) TArray<TSubclassOf<AInteractiveCharacter>> UsedCharacters;
  UPROPERTY() TArray<AInteractiveCharacter*> PlacableCharacters;

protected:
  UFUNCTION() void ConnectionHappened();

public:
  ATurnBasedBeastAI();

  //void BeginPlay() override;

  //void SetPawn(APawn * InPawn) override;

  const ARegistrationManager* GetUsedManager() const;
};
