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

FVector UMovement::LerpLocationWithCurve(float Alpha, FVector Start, FVector Finish, const UCurveFloat* Curve, float StartTime, float FinishTime, bool GlobalDeviation, bool ZDeviation)
{
  check(Curve);
  check(FinishTime >= StartTime);
  check(Alpha >= 0.f);
  if (Alpha > 1.f) Alpha = 1.f;

  float LerpTime = (StartTime * (1 - Alpha)) + (FinishTime * Alpha);
  FVector BasicLerp = (Start * (1 - Alpha)) + (Finish * Alpha);
  float DeviationScale = Curve->GetFloatValue(LerpTime);

  if (GlobalDeviation)
  {
    if (!ZDeviation) UE_LOG(LogTemp, Error, TEXT("GlobalDeviation can work only with a z-axis deviation!"));
    BasicLerp.Z += DeviationScale;
    return BasicLerp;
  }

  FVector DeltaVector = Finish - Start;

  if (DeltaVector.IsNearlyZero()) return Finish;

  FVector XYProjection = DeltaVector;

  if (ZDeviation)
  {
    XYProjection.Z = 0;
  }
  else
  {
    XYProjection.X = 0;
  }

  if (XYProjection.IsNearlyZero())
  {
    UE_LOG(LogTemp, Error, TEXT("The vector from start to finish is almost collinear to the deviation axes"));
    return BasicLerp;
  }

  FVector XYProjectionPerp = DeltaVector ^ XYProjection;
  FVector DeviationUnit = XYProjectionPerp ^ DeltaVector;
  if (!DeviationUnit.Normalize())
  {
    UE_LOG(LogTemp, Error, TEXT("The vector from start to finish is almost collinear to the deviation axes"));
    return BasicLerp;
  }

  DeviationUnit *= DeviationScale;

  return BasicLerp + DeviationUnit;
}
