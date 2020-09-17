// ...

#include "TurnBasedManager.h"

void ATurnBasedManager::JoinLast(AController* NewController)
{
  // check if it supports turn based interface
  // call ConnectToEvent to spawn objects and give TurnWaiter
  // add to JoinedControllers
}

void ATurnBasedManager::NextPhase()
{
  // make everything needed before next phase
  switch (CurrentTurnPhase)
  {
  case ETurnPhase::Start:
    break;
  case ETurnPhase::AbilitiesSetup:
    break;
  case ETurnPhase::AbilitiesEffect:
    break;
  case ETurnPhase::End:
    break;
  default:
    break;
  }
}