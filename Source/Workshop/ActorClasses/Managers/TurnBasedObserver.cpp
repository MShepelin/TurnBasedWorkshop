// Fill out your copyright notice in the Description page of Project Settings.

#include "TurnBasedObserver.h"
#include "Workshop/Interfaces/TurnBasedInterface.h"

void ATurnBasedObserver::OnConnectToManager(ATurnBasedManager *Manager)
{

}

void ATurnBasedObserver::OnGetTurnControl()
{

}

void ATurnBasedObserver::OnLoseTurnControl()
{

}

void ATurnBasedObserver::PossessedBy(AController *NewController)
{
  Super::PossessedBy(NewController);

  TurnBasedController.SetObject(NewController);
  TurnBasedController.SetInterface(Cast<ITurnBasedInterface>(NewController));
  if (nullptr == TurnBasedController.GetInterface())
  {
    return;
  }

  TurnBasedController->OnConnectToManager();
}

void ATurnBasedObserver::UnPossessed()
{
  Super::UnPossessed();

  TurnBasedController.SetObject(nullptr);
  TurnBasedController.SetInterface(nullptr);
}
