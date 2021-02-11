// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Overlay.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/Button.h"
#include "Blueprint/WidgetTree.h"
#include "GeneratedCodeHelpers.h"
#include "Components/RichTextBlock.h"
#include "Workshop/ActorClasses/Managers/RegistrationManager.h"
#include "HAL/ThreadSafeCounter.h"
#include "../MixedProgressBar.h"
#include "Workshop/Types/InteractiveObjectData/InteractiveCore.h"
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
  UPROPERTY(EditDefaultsOnly)
  TSubclassOf<UMixedProgressBar> BarSlotClass;

  UPROPERTY() TArray<UAbilitySlot*> VerticalBoxSlots;
  UPROPERTY(meta = (BindWidget)) UVerticalBox* AbilitiesLayout;

  UPROPERTY() TArray<UMixedProgressBar*> BarsSlots;
  UPROPERTY(meta = (BindWidget)) UVerticalBox* BarsLayout;

public:
  UPROPERTY(meta = (BindWidget)) UButton* NextPhaseButton;
  UPROPERTY(meta = (BindWidget)) URichTextBlock* PhaseText;
  UPROPERTY(meta = (BindWidget)) URichTextBlock* NextPhaseText;

  UPROPERTY(meta = (BindWidget)) UButton* TurnSwapButton;
  UPROPERTY(meta = (BindWidget)) URichTextBlock* SwapText;

  UPROPERTY(meta = (BindWidget)) URichTextBlock* MassageText;
  UPROPERTY(meta = (BindWidget)) URichTextBlock* NameText;

  UPROPERTY(BlueprintReadOnly) FText SwapIsActiveText = FText::FromString("Stop Swap");
  UPROPERTY(BlueprintReadOnly) FText SwapIsInactiveText = FText::FromString("Start Swap");;

protected:
  UFUNCTION() void AddAbilitySlot();
  UFUNCTION() void AddBarSlot();

  UFUNCTION() void RemoveAbilitySlot();
  UFUNCTION() void RemoveBarSlot();

public:
  UFUNCTION() void NativePreConstruct() override;

  UFUNCTION() void HideAbilitySlots();
  UFUNCTION() void HideBarsSlots();
  UFUNCTION() void ShowAbilitySlots();
  UFUNCTION() void ShowBarsSlots();

  UFUNCTION() void FailToInteract();
  UFUNCTION() void ShowBasicText();

  UFUNCTION() void FillAbilitySlots(const TArray<AInteractiveAbility*>& Abilities, bool bIsControlled);
  UFUNCTION() void FillBarSlots(const TArray<FBar>& Bars);

  UFUNCTION() void SetInteractiveObjectData(const FInteractiveCore& Data);
  UFUNCTION() void HideName();
};

//++++ add function to set abilities of new character:
// for each ability spot (add/remove some if needed) setchosenability
//???? add hud to store this widget