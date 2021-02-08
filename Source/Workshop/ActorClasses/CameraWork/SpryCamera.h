// ...

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "SpryCamera.generated.h"

class AInteractiveCharacter;

UCLASS(Blueprintable)
class WORKSHOP_API ASpryCamera : public APawn
{
  GENERATED_BODY()

protected:
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) UCameraComponent* Camera;
  UPROPERTY(VisibleDefaultsOnly) UWidgetInteractionComponent* WidgetInteraction;

  UPROPERTY() APlayerController* PlayerController = nullptr;
  UPROPERTY(EditDefaultsOnly) float ApproachDistance = 0.f;

  TQueue<TTuple<AActor*, float>> ApproachTasks;

private:
  UPROPERTY() FVector HiddenLocation;

  UPROPERTY() uint8 bIsPossesed : 1;

protected:
  UPROPERTY(EditDefaultsOnly)
  uint8 bApproachType : 1;

public:
  ASpryCamera();

  virtual void OnConstruction(const FTransform & Transform) override;

  virtual void PossessedBy(AController * NewController) override;

  virtual void UnPossessed() override;

  UFUNCTION(BlueprintNativeEvent)
  void OnApproach(AActor* ActorToApproach, float AvailableTime);
  virtual void OnApproach_Implementation(AActor* ActorToApproach, float AvailableTime);

  UFUNCTION(BlueprintCallable)
  FVector GetHiddenLocation() const;

  UFUNCTION(BlueprintCallable)
  FVector GetCameraDirection() const;

  UFUNCTION(BlueprintCallable)
  FVector GetCameraLocation() const;

  UFUNCTION() void PlayerPressedClick();

  UFUNCTION() void PlayerReleasedClick();
};
