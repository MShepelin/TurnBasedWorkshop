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

private:
  UPROPERTY() FVector HiddenLocation;

  bool bIsPossesed = false;

protected:
  UPROPERTY(EditDefaultsOnly) UCameraComponent* Camera;
  UPROPERTY(VisibleDefaultsOnly) UWidgetInteractionComponent* WidgetInteraction;

  APlayerController* PlayerController = nullptr;

public:
  UPROPERTY(EditAnywhere, Category = "TurnBased")
  UInstancedStaticMeshComponent* SpawnLocations;

  ASpryCamera();

  virtual void OnConstruction(const FTransform & Transform) override;

  virtual void PossessedBy(AController * NewController) override;

  virtual void UnPossessed() override;

  virtual void Tick(float DeltaSeconds) override;

  /* REWORK
  UFUNCTION(BlueprintNativeEvent)
  void OnCharacterResolvesAbility(AInteractiveCharacter* CurrentCharacter);
  virtual void OnCharacterResolvesAbility_Implementation(AInteractiveCharacter* CurrentCharacter);
  */

  UFUNCTION(BlueprintCallable)
  FVector GetHiddenLocation() const;

  UFUNCTION(BlueprintCallable)
  FVector GetCameraDirection() const;

  UFUNCTION(BlueprintCallable)
  FVector GetCameraLocation() const;

  UFUNCTION() void PlayerPressedClick();

  UFUNCTION() void PlayerReleasedClick();
};
