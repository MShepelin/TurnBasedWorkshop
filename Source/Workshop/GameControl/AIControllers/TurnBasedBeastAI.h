// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Workshop/ActorClasses/Managers/RegistrationManager.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveCharacter.h"
#include "TurnBasedBeastAI.generated.h"

UCLASS(Blueprintable)
class WORKSHOP_API ATurnBasedBeastAI : public APlayerController
{
  GENERATED_BODY()

protected:
  UPROPERTY() ARegistrationManager* UsedManager = nullptr;

public:
  //ATurnBasedBeastAI();

  //void BeginPlay() override;

  //void SetPawn(APawn * InPawn) override;

  const ARegistrationManager* GetUsedManager() const;
};
