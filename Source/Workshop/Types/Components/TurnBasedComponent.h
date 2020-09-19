// ...

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TurnBasedComponent.generated.h"


class ATurnBasedManager;


// Allows to connect with ATurnBasedManager and controll allocated turn
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WORKSHOP_API UTurnBasedComponent : public UActorComponent
{
  GENERATED_BODY()

private:
  UPROPERTY() bool bIsTurnControlled;
  UPROPERTY() ATurnBasedManager* Manager;

public:
  UFUNCTION(BlueprintCallable)
  bool IsTurnControlled() const;

  UFUNCTION(BlueprintCallable)
  void NextPhase();

  UFUNCTION(BlueprintCallable)
  bool IsManaged() const;

  // ATurnBasedManager changes UTurnBasedComponent
  // and gives permission to control a turn.
  friend class ATurnBasedManager;
};
