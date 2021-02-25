// Copyright © Dmitriy Shepelin 2021. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Controller.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveCharacter.h"
#include "Workshop/ActorClasses/Managers/TurnBasedManager.h"
#include "Workshop/Interfaces/TurnBasedInterface.h"
#include "Workshop/ActorClasses/Managers/TurnBasedObserver.h"
#include "FightAI.generated.h"

UCLASS(Blueprintable)
class WORKSHOP_API AFightAI : public AController, public ITurnBasedInterface
{
  GENERATED_BODY()

public:
  UPROPERTY() ATurnBasedManager* UsedManager;

public:
  AFightAI();

  void OnConnectToManager_Implementation() override;

  void OnGetTurnControl_Implementation() override;
};
