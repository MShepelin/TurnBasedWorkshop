// ...

#include "TurnBasedComponent.h"
#include "Workshop/ActorClasses/Managers/TurnBasedManager.h"

void UTurnBasedComponent::ConnectionHappened()
{
  ConnectDelegate.ExecuteIfBound();
}

bool UTurnBasedComponent::IsTurnControlled() const
{
  return bIsTurnControlled;
}

void UTurnBasedComponent::NextPhase()
{
  check(Manager != nullptr);

  if (!bIsTurnControlled)
  {
    return;
  }

  Manager->NextPhase();
}

bool UTurnBasedComponent::IsManaged() const
{
  return (Manager != nullptr);
}

ATurnBasedManager* UTurnBasedComponent::GetManager()
{
  return Manager;
}
