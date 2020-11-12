// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CameraController.h"
#include "Components/WidgetComponent.h"
#include "Workshop/UI/TurnBasedEvent/AbilitiesWidget.h"
#include "Workshop/Types/Components/TurnBasedComponent.h"
#include "InteractController.generated.h"

class ATurnBasedManager;
class AInteractiveCharacter;

UCLASS(Blueprintable)
class WORKSHOP_API AInteractController : public ACameraController
{
	GENERATED_BODY()

protected:
  UPROPERTY(VisibleDefaultsOnly) UTurnBasedComponent* TurnControl;

  UPROPERTY() TArray<AInteractiveCharacter*> PlacableCharacters;
  TArray<TPair<int32, FTransform>> CharactersSpawnTransforms;

  AInteractiveCharacter* FirstToSwap[2];
  UPROPERTY() bool bSwapModeIsActive;

  UPROPERTY() UAbilitiesWidget* UsedAbilitiesWidget;

  // Tries to pick an Interactive object
  UFUNCTION() void StartInteract();
  UFUNCTION() void StopInteract();

  UFUNCTION() void ConnectionHappened();
  
  // If SwapMode is active it will be turned off,
  // else it will be turned on
  UFUNCTION() void TurnSwapMode();

public:
  AInteractController();

  void BeginPlay() override;

  void SetupInputComponent() override;

  void PostInitializeComponents() override;

  UFUNCTION(BlueprintCallable)
  void ResolveCharactersAbilities();

  UFUNCTION(BlueprintCallable)
  void LinkWithAbilitiesWidget(UAbilitiesWidget* AbilitiesWidget);

  UFUNCTION(BlueprintCallable)
  UAbilitiesWidget* GetAbilitiesWidget();

  // Should be called before any other functions 
  // when all objects used by controller are prepared
  
  //UFUNCTION(BlueprintCallable)
  //void ObjectsReady(ATurnBasedManager* EventManager);

  UFUNCTION() void PrepareCharacters();

  UFUNCTION(BlueprintCallable)
  void AddSpawnTransform(FTransform NewSpawn, int32 Order);
};
