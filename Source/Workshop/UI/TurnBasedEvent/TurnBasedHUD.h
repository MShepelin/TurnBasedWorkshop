// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AbilitiesWidget.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveObject.h"
#include "Workshop/ActorClasses/Managers/RegistrationManager.h"
#include "TurnBasedHUD.generated.h"

UCLASS()
class WORKSHOP_API ATurnBasedHUD : public AHUD
{
	GENERATED_BODY()

private:
  UPROPERTY() ARegistrationManager* CentralManager;
  UPROPERTY() UAbilitiesWidget* AbilitiesWidget;

  //++++ information widget
  //++++ menu widget

public:
  UFUNCTION(BlueprintCallable)
  void SetCentralManager(ARegistrationManager* NewManager);

  UFUNCTION(BlueprintCallable)
  void HideAbilities();

  UFUNCTION(BlueprintCallable)
  void ShowAbilities();

  UFUNCTION(BlueprintCallable)
  UAbilitiesWidget* GetAbilitiesWidget();

  UFUNCTION(BlueprintCallable)
  void SetAbilitiesWidget(UAbilitiesWidget* NewWidget);
};
