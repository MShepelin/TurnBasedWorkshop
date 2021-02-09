// ...

#pragma once

#include "Workshop/ActorClasses/Managers/TurnBasedObserver.h"
#include "TurnBasedInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UTurnBasedInterface : public UInterface
{
  GENERATED_BODY()
};

class ITurnBasedInterface
{
  GENERATED_BODY()

protected:
  UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
  void OnConnectToManager();
  virtual void OnConnectToManager_Implementation();

  friend ATurnBasedObserver;
};
