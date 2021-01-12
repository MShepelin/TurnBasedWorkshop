// ...

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Workshop/Types/Bar.h"
#include "Workshop/Types/BarColorScheme.h"
#include "Workshop/ActorClasses/Managers/RegistrationManager.h"
#include "MixedProgressBar.generated.h"

UCLASS(Abstract)
class WORKSHOP_API UMixedProgressBar : public UUserWidget
{
  GENERATED_BODY()

public:
  UPROPERTY(meta = (BindWidget)) UImage* BarImage;

  UPROPERTY(EditAnywhere, BlueprintReadOnly) FBar BarToDraw;

  UPROPERTY(EditAnywhere, BlueprintReadOnly) FBarColorScheme ColorScheme;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) USlateBrushAsset* ActiveSlotsBrush;

  UPROPERTY(BlueprintReadOnly) FText BackgroundText;

public:
  UFUNCTION(BlueprintCallable) void SetBar(const FBar& Bar);
  UFUNCTION(BlueprintCallable) void SetBarAndText(const FBar& Bar, ARegistrationManager *Manager);
};
