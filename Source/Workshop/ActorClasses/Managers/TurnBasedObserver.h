// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Workshop/Types/Nonblueprintable/GameConstants.h"
#include "TurnBasedManager.h"
#include "TurnBasedObserver.generated.h"

class ITurnBasedInterface;

UCLASS(Blueprintable)
class WORKSHOP_API ATurnBasedObserver : public APawn
{
  GENERATED_BODY()

  UPROPERTY() TScriptInterface<ITurnBasedInterface> TurnBasedController = nullptr;

public:
  virtual void PossessedBy(AController *NewController) override;

  virtual void UnPossessed() override;

  void OnConnectToManager(ATurnBasedManager *Manager);

  void OnGetTurnControl();

  void OnLoseTurnControl();
};
