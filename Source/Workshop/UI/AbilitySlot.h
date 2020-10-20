// ...

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/SizeBox.h"
#include "Components/ScaleBox.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Workshop/ActorClasses/Managers/RegistrationManager.h"
#include "AbilitySlot.generated.h"

class AInteractiveAbility;

UCLASS(Abstract)
class WORKSHOP_API UAbilitySlot : public UUserWidget
{
  GENERATED_BODY()

private:
  UPROPERTY(EditDefaultsOnly, Category = "SlotSettings") 
  UTexture* EmptySlot;

  UPROPERTY(meta = (BindWidget)) UScaleBox* IconScaleBox;
  UPROPERTY(meta = (BindWidget)) USizeBox* IconSizeBox;

  UPROPERTY(EditDefaultsOnly, meta = (BindWidget)) UButton* AbilityButton;
  UPROPERTY(meta = (BindWidget)) UImage* AbilityIcon;

  UPROPERTY(meta = (BindWidget)) UTextBlock* EffectsInfo;

  UPROPERTY() AInteractiveAbility* ChosenAbility;

public:
  UFUNCTION(BlueprintCallable)
  void SetMaxSize(float IconMaxSize, float IconMinSize);

  void SetChosenAbility(AInteractiveAbility* NewAbility, ARegistrationManager* UsedManager);

  UFUNCTION() void AbilityClicked();

  void NativePreConstruct() override;
};
