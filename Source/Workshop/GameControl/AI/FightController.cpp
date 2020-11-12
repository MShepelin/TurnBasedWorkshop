// ...

#include "FightController.h"

AFightController::AFightController()
{
  TurnControl = CreateDefaultSubobject<UTurnBasedComponent>(TEXT("TurnControl"));
  AddOwnedComponent(TurnControl);
  TurnControl->ConnectDelegate.BindUObject(this, &AFightController::ConnectionHappened);
}

void AFightController::ConnectionHappened()
{

}
