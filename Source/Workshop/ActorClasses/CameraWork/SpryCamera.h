// ...

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "SpryCamera.generated.h"


class AInteractiveCharacter;


UCLASS()
class WORKSHOP_API ASpryCamera : public APawn
{
  GENERATED_BODY()

private:
  UPROPERTY() FVector HiddenLocation;

  bool bIsPossesed = false;

protected:
  UPROPERTY(VisibleDefaultsOnly) UCameraComponent* Camera;
  UPROPERTY(VisibleDefaultsOnly) UWidgetInteractionComponent* WidgetInteraction;

  APlayerController* PlayerController = nullptr;

public:
  ASpryCamera();

  virtual void OnConstruction(const FTransform & Transform) override;

  virtual void PossessedBy(AController * NewController) override;

  virtual void UnPossessed() override;

  virtual void Tick(float DeltaSeconds) override;

  UFUNCTION(BlueprintNativeEvent)
  void OnCharacterResolvesAbility(AInteractiveCharacter* CurrentCharacter);
  virtual void OnCharacterResolvesAbility_Implementation(AInteractiveCharacter* CurrentCharacter);

  UFUNCTION(BlueprintCallable)
  FVector GetHiddenLocation() const;

  UFUNCTION(BlueprintCallable)
  FVector GetCameraDirection() const;

  UFUNCTION() void PlayerPressedClick();

  UFUNCTION() void PlayerReleasedClick();
};
