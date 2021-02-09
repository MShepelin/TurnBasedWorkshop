// ...

#include "TurnBasedInterface.h"

void ITurnBasedInterface::OnConnectToManager_Implementation()
{
  UE_LOG(LogTemp, Warning, TEXT("Successful connection"));
}

void ITurnBasedInterface::OnDisconnectFromManager_Implementation()
{
  UE_LOG(LogTemp, Warning, TEXT("Disconnection"));
}

void ITurnBasedInterface::OnLoseTurnControl_Implementation()
{
  UE_LOG(LogTemp, Warning, TEXT("Lost control"));
}

void ITurnBasedInterface::OnGetTurnControl_Implementation()
{
  UE_LOG(LogTemp, Warning, TEXT("Got control"));
}
