// Copyright © Dmitriy Shepelin 2021. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Workshop/Types/Bar.h"
#include "Workshop/Types/BarColorScheme.h"
#include "Workshop/ActorClasses/Managers/RegistrationManager.h"
#include "Workshop/WorkshopGameModeBase.h"
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

protected:
  virtual int32 NativePaint(
    const FPaintArgs& Args, 
    const FGeometry& AllottedGeometry, 
    const FSlateRect& MyCullingRect, 
    FSlateWindowElementList& OutDrawElements, 
    int32 LayerId, 
    const FWidgetStyle& InWidgetStyle, 
    bool bParentEnabled) const override;

public:
  UFUNCTION(BlueprintCallable) void SetBar(const FBar& Bar);

  UFUNCTION(BlueprintCallable) void SetBarAndText(const FBar& Bar, AWorkshopGameModeBase *GameMode);
};
