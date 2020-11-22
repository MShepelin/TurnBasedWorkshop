// Fill out your copyright notice in the Description page of Project Settings.

#include "ParseData.h"
#include "Workshop/ActorClasses/Managers/RegistrationManager.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Slate/SlateBrushAsset.h"

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
  const FBar& UsedBar = BarWidget->BarToDraw;

  FVector2D Size = USlateBlueprintLibrary::GetLocalSize(BarWidget->BarImage->GetPaintSpaceGeometry());
  FVector2D Position = USlateBlueprintLibrary::GetLocalTopLeft(BarWidget->BarImage->GetPaintSpaceGeometry());

  for (size_t LimitIndex = 1; LimitIndex < UsedBar.BarLimits.Num() - 1; LimitIndex += 2)
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
  }
}
