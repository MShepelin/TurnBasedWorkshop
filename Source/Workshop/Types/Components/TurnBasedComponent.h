// ...

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TurnBasedComponent.generated.h"

class ATurnBasedManager;

DECLARE_DELEGATE(FStandartDelegateSignature)

// Allows to connect with ATurnBasedManager and controll allocated turn
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WORKSHOP_API UTurnBasedComponent : public UActorComponent
{
  GENERATED_BODY()

public:
  FStandartDelegateSignature ConnectDelegate;
  FStandartDelegateSignature TurnIsTakenUnderControl;
  FStandartDelegateSignature TurnIsOutOfControl;

private:
  UPROPERTY() bool bIsTurnControlled;
  UPROPERTY() ATurnBasedManager* Manager;

  UFUNCTION() void ConnectionHappened();

  UFUNCTION() void ChangeControl(bool bComponentHasControl);
public:

  UFUNCTION(BlueprintCallable)
  bool IsTurnControlled() const;

  UFUNCTION(BlueprintCallable)
  void NextPhase();

  UFUNCTION(BlueprintCallable)
  bool IsManaged() const;

  UFUNCTION(BlueprintCallable)
  ATurnBasedManager* GetManager();

  // ATurnBasedManager changes UTurnBasedComponent
  // and gives permission to control a turn.
  friend class ATurnBasedManager;
};
