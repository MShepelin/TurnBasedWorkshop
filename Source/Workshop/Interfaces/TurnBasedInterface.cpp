// ...

#include "TurnBasedInterface.h"

void ITurnBasedInterface::OnConnectToManager_Implementation()
{
  UE_LOG(LogTemp, Warning, TEXT("Successful connection"));
}

void ITurnBasedInterface::OnDisconnectFromManager_Implementation()
{
  UE_LOG(LogTemp, Warning, TEXT("Got control"));
}

void ITurnBasedInterface::OnLoseTurnControl_Implementation()
{

}

void ITurnBasedInterface::OnGetTurnControl_Implementation()
{

}
