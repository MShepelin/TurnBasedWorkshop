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
  FVector2D Size = USlateBlueprintLibrary::GetLocalSize(BarWidget->BarImage->GetPaintSpaceGeometry());
  FVector2D Position = USlateBlueprintLibrary::GetLocalTopLeft(BarWidget->BarImage->GetPaintSpaceGeometry());
  Size.X /= 2;

  UWidgetBlueprintLibrary::DrawBox
  (
    Context,
    Position,
    Size,
    BarWidget->ActiveSlotsBrush,
    FLinearColor(255, 255, 255)
  );
}
