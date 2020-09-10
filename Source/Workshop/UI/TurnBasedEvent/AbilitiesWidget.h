// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Overlay.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Blueprint/WidgetTree.h"
#include "GeneratedCodeHelpers.h"
#include "AbilitiesWidget.generated.h"


class UAbilitySlot;
class AInteractiveAbility;


UCLASS(Abstract)
class WORKSHOP_API UAbilitiesWidget : public UUserWidget
{
  GENERATED_BODY()

protected:
  UPROPERTY(EditDefaultsOnly)
  TSubclassOf<UAbilitySlot> AbilitySlotClass;

  UPROPERTY() TArray<UAbilitySlot*> VerticalBoxSlots;
  UPROPERTY(meta = (BindWidget)) UVerticalBox* AbilitiesLayout; //, meta = (BindWidget)

protected:
  void AddAbilitySlot();

  void RemoveAbilitySlot();

public:
  void FillAbilitySlots(const TArray<AInteractiveAbility*>& Abilities);

  void NativePreConstruct() override;
};

//++++ add function to set abilities of new character:
// for each ability spot (add/remove some if needed) setchosenability
//???? add hud to store this widget
