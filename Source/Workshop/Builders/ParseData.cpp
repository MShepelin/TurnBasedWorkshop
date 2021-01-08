// Fill out your copyright notice in the Description page of Project Settings.

#include "ParseData.h"
#include "Workshop/ActorClasses/Managers/RegistrationManager.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Workshop/Types/Nonblueprintable/GameConstants.h"
#include "Slate/SlateBrushAsset.h"
#include "Kismet/KismetMathLibrary.h"

void UParseData::DrawBar(const UMixedProgressBar* BarWidget, FPaintContext& Context)
{
  //++++ BRING CUSTOMIZATION VALUES TO EDITOR
  const FBar& UsedBar = BarWidget->BarToDraw;

  FVector2D Size = USlateBlueprintLibrary::GetLocalSize(BarWidget->BarImage->GetPaintSpaceGeometry());
  FVector2D Position = USlateBlueprintLibrary::GetLocalTopLeft(BarWidget->BarImage->GetPaintSpaceGeometry());

  // Limits in the form of float numbers from 0 to 1
  TArray<float> BarLimits;
  for (int8 Limit : UsedBar.BarLimits)
  {
    BarLimits.Add(Limit);
  }

  for (int LimitIndex = 1; LimitIndex + 1 < BarLimits.Num(); LimitIndex += 2)
  {
    BarLimits[LimitIndex + 1] /= BAR_MAX_VALUE;
    BarLimits[LimitIndex] /= BAR_MAX_VALUE;

    // Odd indexes match the starts of active zones
    // Even indexes match the ends

    FVector2D ZoneSize = Size;
    ZoneSize.X = ZoneSize.X * (BarLimits[LimitIndex + 1] - BarLimits[LimitIndex]);
    FVector2D ZonePosition = Position;
    ZonePosition.X += Size.X * BarLimits[LimitIndex];

    UWidgetBlueprintLibrary::DrawBox
    (
      Context,
      ZonePosition,
      ZoneSize,
      BarWidget->ActiveSlotsBrush,
      FLinearColor(255, 255, 255)
    );

    UWidgetBlueprintLibrary::DrawTextFormatted(
      Context,
      FText::FromString(FString::FromInt(UsedBar.BarLimits[LimitIndex])),
      { ZonePosition.X, ZonePosition.Y - SMALL_PADDING },
      BarWidget->BarTextFont,
      FONT_SMALL_SIZE,
      FName(TEXT("Regular")),
      BarWidget->WidgetColor);

    if (BarLimits[LimitIndex + 1] >= 1.f)
    {
      break;
    }

    UWidgetBlueprintLibrary::DrawTextFormatted(
      Context,
      FText::FromString(FString::FromInt(UsedBar.BarLimits[LimitIndex + 1])),
      { Position.X + Size.X * BarLimits[LimitIndex + 1], ZonePosition.Y - SMALL_PADDING },
      BarWidget->BarTextFont,
      FONT_SMALL_SIZE,
      FName(TEXT("Regular")),
      BarWidget->WidgetColor);
  }

  // Current value of the bar in the form of a float number from 0 to 1
  float CurrentValue = ((float)UsedBar.CurrentValue) / BAR_MAX_VALUE;

  FVector2D CurrentValuePosition = Position;
  CurrentValuePosition.X += CurrentValue * Size.X;

  UWidgetBlueprintLibrary::DrawLine(
    Context,
    CurrentValuePosition, 
    { CurrentValuePosition.X, CurrentValuePosition.Y + Size.Y },
    BarWidget->WidgetColor,
    true, 
    LINE_THICKNESS);

  UWidgetBlueprintLibrary::DrawTextFormatted(
    Context,
    FText::FromString(FString::FromInt(UsedBar.CurrentValue)),
    { (UsedBar.CurrentValue > (BAR_MAX_VALUE / 2)) ? CurrentValuePosition.X - SMALL_PADDING * 1.5f : CurrentValuePosition.X + SMALL_PADDING * 0.1f, CurrentValuePosition.Y },
    BarWidget->BarTextFont,
    FONT_MIDDLE_SIZE,
    FName(TEXT("Regular")),
    BarWidget->WidgetColor);
}
