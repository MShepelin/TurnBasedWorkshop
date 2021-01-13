// ...

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/SizeBox.h"
#include "Components/ScaleBox.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "Components/Button.h"
#include "Components/RichTextBlock.h"
#include "Workshop/ActorClasses/Managers/RegistrationManager.h"
#include "AbilitySlot.generated.h"

class AInteractiveAbility;

UCLASS(Abstract)
class WORKSHOP_API UAbilitySlot : public UUserWidget
{
  GENERATED_BODY()

protected:
  UPROPERTY(EditDefaultsOnly, Category = "SlotSettings") 
  UTexture* EmptySlot;

  UPROPERTY(EditDefaultsOnly, meta = (BindWidget)) UButton* AbilityButton;
  UPROPERTY(meta = (BindWidget)) UImage* AbilityIcon;

  UPROPERTY(meta = (BindWidget)) URichTextBlock* EffectsInfo;

  UPROPERTY() AInteractiveAbility* ChosenAbility;

  UPROPERTY(BlueprintReadOnly) bool bIsControlledByPlayer;

public:
  void SetChosenAbility(AInteractiveAbility* NewAbility, ARegistrationManager* UsedManager, bool bIsControlled);

  UFUNCTION() void AbilityClicked();

  void NativePreConstruct() override;
};
