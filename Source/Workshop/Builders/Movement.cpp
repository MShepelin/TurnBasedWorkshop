// Copyright © Dmitriy Shepelin 2021. MIT License.

#include "Movement.h"
#include "Workshop/ActorClasses/Managers/RegistrationManager.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Workshop/Types/Nonblueprintable/GameConstants.h"
#include "Slate/SlateBrushAsset.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Kismet/KismetMathLibrary.h"

void UMovement::CompleteSplineToDestination(UCurveFloat* Curve, USplineComponent* Spline, FVector Destination)
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

void UMovement::MoveFlipbookAcrossSpline(USplineComponent*& ObjectSplineReference, USplineComponent* SplineToUse, UTimelineComponent* Timeline, float Duration)
{
  if (!Duration || FGenericPlatformMath::IsNaN(Duration))
  {
    UE_LOG(LogTemp, Error, TEXT("Incorrect Duration for MoveFlipbookAcrossSpline!"));
    return;
  }

  ObjectSplineReference = SplineToUse;
  Timeline->SetPlayRate(1 / Duration);
  Timeline->PlayFromStart();
}
