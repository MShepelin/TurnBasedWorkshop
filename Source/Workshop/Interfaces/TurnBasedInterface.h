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

public:
  UFUNCTION(BlueprintNativeEvent)
  void OnConnectToManager();
  virtual void OnConnectToManager_Implementation();

  UFUNCTION(BlueprintNativeEvent)
  void OnDisconnectFromManager();
  virtual void OnDisconnectFromManager_Implementation();

  UFUNCTION(BlueprintNativeEvent)
  void OnGetTurnControl();
  virtual void OnGetTurnControl_Implementation();

  UFUNCTION(BlueprintNativeEvent)
  void OnLoseTurnControl();
  virtual void OnLoseTurnControl_Implementation();

  friend ATurnBasedObserver;
};
