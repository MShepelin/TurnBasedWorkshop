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

  UPROPERTY() bool bIsTurnControlled;

  // ATurnBasedManager changes UTurnBasedComponent
  // and gives permission to control a turn.
  //friend class ATurnBasedManager;
};
