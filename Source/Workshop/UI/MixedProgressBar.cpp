// Copyright © Dmitriy Shepelin 2021. MIT License.

#include "MixedProgressBar.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UMixedProgressBar::SetBar(const FBar& Bar)
{
  BarToDraw = Bar;
}

void UMixedProgressBar::SetBarAndText(const FBar& Bar, AWorkshopGameModeBase *GameMode)
{
  BarToDraw = Bar;
  BackgroundText = FText::FromString(GameMode->GetStatNameByID(BarToDraw.StatID));
}

int32 UMixedProgressBar::NativePaint(
  const FPaintArgs& Args,
  const FGeometry& AllottedGeometry,
  const FSlateRect& MyCullingRect,
  FSlateWindowElementList& OutDrawElements,
  int32 LayerId,
  const FWidgetStyle& InWidgetStyle,
  bool bParentEnabled) const
{
  FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

  FVector2D Size = USlateBlueprintLibrary::GetLocalSize(BarImage->GetPaintSpaceGeometry());
  FVector2D Position = USlateBlueprintLibrary::GetLocalTopLeft(BarImage->GetPaintSpaceGeometry());

  // Limits in the form of float numbers from 0 to 1
  TArray<float> BarLimits;
  for (int8 Limit : BarToDraw.BarLimits)
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
      ActiveSlotsBrush,
      ColorScheme.ActiveColor
    );

    UWidgetBlueprintLibrary::DrawTextFormatted(
      Context,
      FText::FromString(FString::FromInt(BarToDraw.BarLimits[LimitIndex])),
      { ZonePosition.X, ZonePosition.Y - ColorScheme.TopPadding },
      ColorScheme.TextFont,
      ColorScheme.FontSizeNormal,
      ColorScheme.FontType,
      ColorScheme.TextColor);

    if (BarLimits[LimitIndex + 1] >= 1.f)
    {
      break;
    }

    UWidgetBlueprintLibrary::DrawTextFormatted(
      Context,
      FText::FromString(FString::FromInt(BarToDraw.BarLimits[LimitIndex + 1])),
      { Position.X + Size.X * BarLimits[LimitIndex + 1], ZonePosition.Y - ColorScheme.TopPadding },
      ColorScheme.TextFont,
      ColorScheme.FontSizeNormal,
      ColorScheme.FontType,
      ColorScheme.TextColor);
  }

  // Current value of the bar in the form of a float number from 0 to 1
  float CurrentValue = ((float)BarToDraw.CurrentValue) / BAR_MAX_VALUE;

  FVector2D CurrentValuePosition = Position;
  CurrentValuePosition.X += CurrentValue * Size.X;

  UWidgetBlueprintLibrary::DrawTextFormatted(
    Context,
    BackgroundText,
    { Position.X + ColorScheme.PaddingFromLine, CurrentValuePosition.Y },
    ColorScheme.TextFont,
    ColorScheme.FontSizeBig,
    FName("Bold"),
    ColorScheme.BackgroundColor);

  UWidgetBlueprintLibrary::DrawLine(
    Context,
    CurrentValuePosition,
    { CurrentValuePosition.X, CurrentValuePosition.Y + Size.Y },
    ColorScheme.TextColor,
    true,
    LINE_THICKNESS);

  float NumOfDigits = 0;
  for (int8 ValueToCheck = BarToDraw.CurrentValue; ValueToCheck > 0; ValueToCheck /= 10, NumOfDigits += 1.f) {};

  UWidgetBlueprintLibrary::DrawTextFormatted(
    Context,
    FText::FromString(FString::FromInt(BarToDraw.CurrentValue)),
    {
      (BarToDraw.CurrentValue > (BAR_MAX_VALUE / 2)) ? \
        CurrentValuePosition.X - ColorScheme.FontSizeNormal * NumOfDigits * 0.75f - ColorScheme.PaddingFromLine : \
        CurrentValuePosition.X + ColorScheme.PaddingFromLine,
      CurrentValuePosition.Y + Size.Y + ColorScheme.BottomPadding
    },
    ColorScheme.TextFont,
    ColorScheme.FontSizeNormal,
    ColorScheme.FontType,
    ColorScheme.TextColor);

  return Super::NativePaint(
    Args,
    AllottedGeometry,
    MyCullingRect,
    OutDrawElements,
    LayerId,
    InWidgetStyle,
    bParentEnabled);
}
