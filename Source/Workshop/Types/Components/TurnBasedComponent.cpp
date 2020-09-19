// ...

#include "TurnBasedComponent.h"
#include "Workshop/ActorClasses/Managers/TurnBasedManager.h"

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
