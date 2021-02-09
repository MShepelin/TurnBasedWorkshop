// ...

#include "TurnBasedInterface.h"

void ITurnBasedInterface::OnConnectToManager_Implementation()
{
  UE_LOG(LogTemp, Warning, TEXT("Successful connection"));
}
