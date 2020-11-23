// ...

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Workshop/Types/Bar.h"
#include "MixedProgressBar.generated.h"

UCLASS(Abstract)
class WORKSHOP_API UMixedProgressBar : public UUserWidget
{
  GENERATED_BODY()

public:
  UPROPERTY(meta = (BindWidget)) UImage* BarImage;

  UPROPERTY(EditAnywhere, BlueprintReadOnly) UFont* BarTextFont;

  UPROPERTY(EditAnywhere, BlueprintReadOnly) FBar BarToDraw;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) USlateBrushAsset* ActiveSlotsBrush;

public:
  UFUNCTION(BlueprintCallable) void SetBar(FBar& Bar);
};
