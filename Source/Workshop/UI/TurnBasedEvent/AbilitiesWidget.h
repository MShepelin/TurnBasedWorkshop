// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Overlay.h"
#include "Components/HorizontalBox.h"
#include "AbilitiesWidget.generated.h"

UCLASS(Blueprintable, BlueprintType)
class WORKSHOP_API UAbilitiesWidget : public UUserWidget
{
  GENERATED_BODY()

protected:
  UPROPERTY()
  TArray<UOverlay*> AbilitiesSpots;

  UPROPERTY()
  UHorizontalBox* AbilitiesBox;
};

//++++ add function to set abilities of new character:
// for each ability spot (add/remove some if needed) setchosenability
//???? add hud to store this widget
