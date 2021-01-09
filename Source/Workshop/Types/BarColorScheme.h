#pragma once

#include "CoreMinimal.h"
#include "BarColorScheme.generated.h"

USTRUCT(BlueprintType)
struct WORKSHOP_API FBarColorScheme
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadOnly) FLinearColor TextColor;
  UPROPERTY(EditAnywhere, BlueprintReadOnly) FLinearColor ActiveColor;
  UPROPERTY(EditAnywhere, BlueprintReadOnly) FLinearColor PassiveColor;
  UPROPERTY(EditAnywhere, BlueprintReadOnly) FLinearColor BackgroundColor;
  UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 FontSizeBig;
  UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 FontSizeNormal;
  UPROPERTY(EditAnywhere, BlueprintReadOnly) FName FontType;
  UPROPERTY(EditAnywhere, BlueprintReadOnly) UFont* TextFont;
  UPROPERTY(EditAnywhere, BlueprintReadOnly) float TopPadding;
  UPROPERTY(EditAnywhere, BlueprintReadOnly) float BottomPadding;
  UPROPERTY(EditAnywhere, BlueprintReadOnly) float PaddingFromLine;
};
