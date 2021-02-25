// Copyright © Dmitriy Shepelin 2021. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveAbility.h"
#include "Workshop/Types/Effects/EffectData.h"
#include "Workshop/UI/MixedProgressBar.h"
#include "Components/RichTextBlock.h"
#include "Workshop/Types/BarColorScheme.h"
#include "Workshop/ActorClasses/Managers/RegistrationManager.h"
#include "Components/SplineComponent.h"
#include "Curves/CurveFloat.h"
#include "PaperFlipbookComponent.h"
#include "Components/TimelineComponent.h"
#include "Movement.generated.h"

// Functions from UBuildAbility should be added to CustomEffect 
// of AInteractiveAbility to implement needed gameplay.
UCLASS()
class WORKSHOP_API UMovement : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
  UFUNCTION(BlueprintCallable)
  static void CompleteSplineToDestination(UCurveFloat* Curve, USplineComponent* Spline, FVector Destination);

  /** 
   * If a path is defined as a vector from one location to another and a curve which desribes global or relative deviation of this vector, 
   * LerpLocationWithSpline can return the location on this path as if it was passed by Alpha (0 <= Alpha <= 1).
   */
  UFUNCTION(BlueprintCallable)
  static FVector LerpLocationWithCurve(float Alpha, FVector Start, FVector Finish, const UCurveFloat* Curve, FVector DeviationUnit);

  UFUNCTION(BlueprintCallable)
  static FVector GetDeviationUnit(FVector Start, FVector Finish, bool GlobalDeviation = false, bool ZDeviation = true);

  UFUNCTION(BlueprintCallable)
  static void MoveFlipbookAcrossSpline(UPARAM(ref) USplineComponent*& ObjectSplineReference, USplineComponent* SplineToUse, UTimelineComponent* Timeline, float Duration);
};
