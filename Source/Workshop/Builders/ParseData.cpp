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
  const FBarColorScheme& ColorScheme = BarWidget->ColorScheme;
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
      ColorScheme.ActiveColor
    );

    UWidgetBlueprintLibrary::DrawTextFormatted(
      Context,
      FText::FromString(FString::FromInt(UsedBar.BarLimits[LimitIndex])),
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
      FText::FromString(FString::FromInt(UsedBar.BarLimits[LimitIndex + 1])),
      { Position.X + Size.X * BarLimits[LimitIndex + 1], ZonePosition.Y - ColorScheme.TopPadding },
      ColorScheme.TextFont,
      ColorScheme.FontSizeNormal,
      ColorScheme.FontType,
      ColorScheme.TextColor);
  }

  // Current value of the bar in the form of a float number from 0 to 1
  float CurrentValue = ((float)UsedBar.CurrentValue) / BAR_MAX_VALUE;

  FVector2D CurrentValuePosition = Position;
  CurrentValuePosition.X += CurrentValue * Size.X;

  UWidgetBlueprintLibrary::DrawTextFormatted(
    Context,
    BarWidget->BackgroundText,
    { Position.X + ColorScheme.PaddingFromLine, CurrentValuePosition.Y },
    ColorScheme.TextFont,
    ColorScheme.FontSizeNormal,
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
  for (int8 ValueToCheck = UsedBar.CurrentValue; ValueToCheck > 0; ValueToCheck /= 10, NumOfDigits += 1.f) {};

  UWidgetBlueprintLibrary::DrawTextFormatted(
    Context,
    FText::FromString(FString::FromInt(UsedBar.CurrentValue)),
    { 
      (UsedBar.CurrentValue > (BAR_MAX_VALUE / 2)) ? \
        CurrentValuePosition.X - ColorScheme.FontSizeNormal * NumOfDigits * 0.75f - ColorScheme.PaddingFromLine : \
        CurrentValuePosition.X + ColorScheme.PaddingFromLine, 
      CurrentValuePosition.Y + Size.Y + ColorScheme.BottomPadding
    },
    ColorScheme.TextFont,
    ColorScheme.FontSizeNormal,
    ColorScheme.FontType,
    ColorScheme.TextColor);
}

void UParseData::CompleteSplineToDestination(UCurveFloat* Curve, USplineComponent* Spline, FVector Destination)
{
  int32 NumberOfSplinePoints = Spline->GetNumberOfSplinePoints();
  FVector StartPoint = Spline->GetLocationAtSplinePoint(NumberOfSplinePoints - 1, ESplineCoordinateSpace::Type::World);

  TArray<FRichCurveKey> CurveKeys = Curve->FloatCurve.GetCopyOfKeys();
  if (CurveKeys.Num() < 2)
  {
    UE_LOG(LogTemp, Error, TEXT("A Curve must have at least 2 key points to perform CompleteSplineToDestination"));
    return;
  }

  float MinTime = CurveKeys[0].Time;
  float DeltaTime = CurveKeys.Last(0).Time - MinTime;
  float ValueStart = CurveKeys[0].Value;
  for (int32 CurveKeyIndex = 1; CurveKeyIndex < CurveKeys.Num(); ++CurveKeyIndex)
  {
    FRichCurveKey& CurveKey = CurveKeys[CurveKeyIndex];
    float TimeFraction = (CurveKey.Time - MinTime) / DeltaTime;
    FVector NewPointLocation = StartPoint + (Destination - StartPoint) * TimeFraction;
    NewPointLocation.Z += CurveKey.Value - ValueStart;
    Spline->AddSplinePoint(NewPointLocation, ESplineCoordinateSpace::Type::World, false);
  }

  Spline->UpdateSpline();
}
