// ...

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Workshop/GameControl/PlayerControllers/InteractController.h"
#include "NextPhaseWidget.generated.h"

UCLASS(Abstract)
class WORKSHOP_API UNextPhaseWidget : public UUserWidget
{
  GENERATED_BODY()

private:
  UPROPERTY(EditDefaultsOnly, meta = (BindWidget)) UButton* NextPhase;

  UPROPERTY() AInteractiveAbility* ChosenAbility;
  UPROPERTY() AInteractController* PlayerController;

  UFUNCTION() void PushButton();

public:
  void NativePreConstruct() override;
};
