// ...

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "SpryCamera.generated.h"


class AInteractiveCharacter;


UCLASS()
class WORKSHOP_API ASpryCamera : public APawn
{
  GENERATED_BODY()

protected:
  UPROPERTY(VisibleDefaultsOnly) UCameraComponent* Camera;

  UPROPERTY() FVector HiddenLocation;

public:
  ASpryCamera();

  UFUNCTION(BlueprintCallable)
  virtual void OnCharacterResolvesAbility(AInteractiveCharacter* CurrentCharacter);

  UFUNCTION(BlueprintCallable)
  FVector GetHiddenLocation() const;
};
