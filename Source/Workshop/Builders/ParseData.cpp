// Fill out your copyright notice in the Description page of Project Settings.

#include "ParseData.h"
#include "Workshop/ActorClasses/Managers/RegistrationManager.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Workshop/Types/Nonblueprintable/GameConstants.h"
#include "Slate/SlateBrushAsset.h"
#include "Kismet/KismetMathLibrary.h"

/*
FString UParseData::CollectAccumulatedEffects(AInteractiveObject* TargetObject)
{
  ARegistrationManager* Manager = TargetObject->GetManager();

  FString ParsedString = "";
  for (TTuple<int32, FBar> IntegerStat : TargetObject->InteractiveDataCore.IntegerStats)
  {
    ParsedString += Manager->GetStatNameByID(IntegerStat.Get<0>()) + " " + FString::FromInt(IntegerStat.Get<1>().CurrentValue) + "\n";
  }

  return ParsedString;
}
*/

void UParseData::DrawBar(const UMixedProgressBar* BarWidget, FPaintContext& Context)
{
  // BRING CUSTOMIZATION VALUES TO EDITOR
  const FBar& UsedBar = BarWidget->BarToDraw;

  FVector2D Size = USlateBlueprintLibrary::GetLocalSize(BarWidget->BarImage->GetPaintSpaceGeometry());
  FVector2D Position = USlateBlueprintLibrary::GetLocalTopLeft(BarWidget->BarImage->GetPaintSpaceGeometry());

  for (int LimitIndex = 1; LimitIndex < UsedBar.BarLimits.Num() - 1; LimitIndex += 2)
  {
    // Odd indexes match the starts of active zones
    // Even indexes match the ends

    FVector2D ZoneSize = Size;
    ZoneSize.X = ZoneSize.X * (UsedBar.BarLimits[LimitIndex + 1] - UsedBar.BarLimits[LimitIndex]);
    FVector2D ZonePosition = Position;
    ZonePosition.X += Size.X * UsedBar.BarLimits[LimitIndex];

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
      FText::FromString(FString::FromInt(UKismetMathLibrary::FFloor(UsedBar.BarLimits[LimitIndex] * BAR_MAX_VALUE))),
      { ZonePosition.X, ZonePosition.Y - SMALL_PADDING },
      BarWidget->BarTextFont,
      FONT_SMALL_SIZE,
      FName(TEXT("Regular")),
      BarWidget->WidgetColor);

    if (UsedBar.BarLimits[LimitIndex + 1] >= 1.f)
    {
      break;
    }

    UWidgetBlueprintLibrary::DrawTextFormatted(
      Context,
      FText::FromString(FString::FromInt(UKismetMathLibrary::FFloor(UsedBar.BarLimits[LimitIndex + 1] * BAR_MAX_VALUE))),
      { Position.X + Size.X * UsedBar.BarLimits[LimitIndex + 1], ZonePosition.Y - SMALL_PADDING },
      BarWidget->BarTextFont,
      FONT_SMALL_SIZE,
      FName(TEXT("Regular")),
      BarWidget->WidgetColor);
  }

  FVector2D CurrentValuePosition = Position;
  CurrentValuePosition.X += UsedBar.CurrentValue * Size.X;

  UWidgetBlueprintLibrary::DrawLine(
    Context,
    CurrentValuePosition, 
    { CurrentValuePosition.X, CurrentValuePosition.Y + Size.Y },
    BarWidget->WidgetColor,
    true, 
    LINE_THICKNESS);

  UWidgetBlueprintLibrary::DrawTextFormatted(
    Context,
    FText::FromString(FString::FromInt(UKismetMathLibrary::FFloor(UsedBar.CurrentValue * BAR_MAX_VALUE))),
    { (UsedBar.CurrentValue > 0.5) ? CurrentValuePosition.X - SMALL_PADDING * 1.5f : CurrentValuePosition.X + SMALL_PADDING * 0.1f, CurrentValuePosition.Y },
    BarWidget->BarTextFont,
    FONT_MIDDLE_SIZE,
    FName(TEXT("Regular")),
    BarWidget->WidgetColor);
}
