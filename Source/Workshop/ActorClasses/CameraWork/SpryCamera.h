// Copyright © Dmitriy Shepelin 2021. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "../Managers/TurnBasedObserver.h"
#include "SpryCamera.generated.h"

class AInteractiveCharacter;

/** 
  * If a player needs to control a mouse position on a screen to choose objects in the world and interact with widgets
  * without rotating an in-game camera, this pawn can be used.
  */
UCLASS(Blueprintable)
class WORKSHOP_API ASpryCamera : public ATurnBasedObserver
{
  GENERATED_BODY()

protected:
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) UCameraComponent* Camera;
  UPROPERTY(VisibleDefaultsOnly) UWidgetInteractionComponent* WidgetInteraction;

  UPROPERTY() APlayerController* PlayerController = nullptr;

private:
  UPROPERTY() FVector HiddenLocation;

  UPROPERTY() uint8 bIsPossesedByPlayer : 1;

public:
  ASpryCamera();

  // -------------- //
  // APawn override //
  // -------------- //

  virtual void PossessedBy(AController * NewController) override;

  virtual void UnPossessed() override;

  // ----------------------------- //
  // Camera and widget interaction //
  // ----------------------------- //

  /** Get a location relative to the camera to put objects so that they can't be seen by camera */
  UFUNCTION(BlueprintCallable)
  FVector GetHiddenLocation() const;

  /** 
    * If this pawn is controlled by a player, returns a vector from the camera to the player's mouse world location.
    * Otherwise returns the direction of the camera component as vector.
    */
  UFUNCTION(BlueprintCallable)
  FVector GetCameraDirection() const;

  /** Gets the world location of the camera. */
  UFUNCTION(BlueprintCallable)
  FVector GetCameraLocation() const;

  /** Interact with widgets as the player presses left mouse button. */
  UFUNCTION() void PlayerPressedClick(); //++++ rename

  /** Interact with widgets as the player releases left mouse button. */
  UFUNCTION() void PlayerReleasedClick(); //++++ rename

  /** Interact with widgets as the player scrolls the mouse wheel. */
  UFUNCTION() void PlayerScrolledWheel(float ScrollDelta);
};
