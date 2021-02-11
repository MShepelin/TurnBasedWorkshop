// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CameraController.h"
#include "Components/WidgetComponent.h"
#include "Workshop/UI/TurnBasedEvent/AbilitiesWidget.h"
#include "HAL/Runnable.h"
#include <memory>
#include "HAL/ThreadSafeBool.h"
#include "Workshop/Interfaces/TurnBasedInterface.h"
#include "Workshop/ActorClasses/Managers/TurnBasedObserver.h"
#include "InteractController.generated.h"

class ATurnBasedManager;
class AInteractiveCharacter;
class FCharactersResolve;

UCLASS(Blueprintable)
class WORKSHOP_API AInteractController : public ACameraController, public ITurnBasedInterface
{
	GENERATED_BODY()

protected:
  UPROPERTY() ATurnBasedObserver* PossessedObserver;

  size_t FirstToSwap;
  UPROPERTY() bool bSwapModeIsActive;

  UPROPERTY() UAbilitiesWidget* UsedAbilitiesWidget;

  std::shared_ptr<FCharactersResolve> ResolveRunnable;
  FRunnableThread* ResolveThread;

  UPROPERTY() uint8 bCanPick : 1;
  
public:
  FThreadSafeBool bTurnIsControlled; //++++ change to protected
  
protected:
  // Tries to pick an Interactive object
  UFUNCTION() void StartInteract();
  UFUNCTION() void StopInteract();
  UFUNCTION() void ScrollWheel(float ScrollDelta);
  
  UFUNCTION() void PlayerWantsToChangePhase();

  // If SwapMode is active it will be turned off,
  // else it will be turned on
  UFUNCTION() void TurnSwapMode();

  UFUNCTION() void UpdatePhaseInfo();

  UFUNCTION() void CantPickCallback();

  FTimerHandle Handler;

public:
  AInteractController();

  virtual void BeginPlay() override;

  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

  virtual void SetupInputComponent() override;

  virtual void PostInitializeComponents() override;

  virtual void SetPawn(APawn *InPawn) override;

  // ------------------ //
  // TurnBasedInterface //
  // ------------------ //

  virtual void OnConnectToManager_Implementation() override;

  virtual void OnDisconnectFromManager_Implementation() override;

  virtual void OnGetTurnControl_Implementation() override;

  virtual void OnLoseTurnControl_Implementation() override;

  // ------ //
  // Others //
  // ------ //

  UFUNCTION(BlueprintCallable)
  void ResolveCharactersAbilities();

  UFUNCTION(BlueprintCallable)
  void LinkWithAbilitiesWidget(UAbilitiesWidget* AbilitiesWidget);

  UFUNCTION(BlueprintCallable)
  UAbilitiesWidget* GetAbilitiesWidget();
};

class FCharactersResolve : public FRunnable
{
private:
  AInteractController* UsedController = nullptr;

public:
  FCharactersResolve(AInteractController* Controller);

  uint32 Run() override;

  void Exit() override;
};
