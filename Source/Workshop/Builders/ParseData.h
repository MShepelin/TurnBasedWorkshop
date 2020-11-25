// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveAbility.h"
#include "Workshop/Types/Effects/ChangeStatEffectData.h"
#include "Workshop/UI/MixedProgressBar.h"
#include "Components/RichTextBlock.h"
#include "ParseData.generated.h"

// Functions from UBuildAbility should be added to CustomEffect 
// of AInteractiveAbility to implement needed gameplay.
UCLASS()
class WORKSHOP_API UParseData : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
  UFUNCTION(BlueprintCallable)
  static void DrawBar(const UMixedProgressBar* BarWidget, UPARAM(ref) FPaintContext& Context);

  // Parse information about AccumulatedEffects.
  //UFUNCTION(BlueprintCallable)
  //FString CollectAccumulatedEffects(AInteractiveObject* TargetObject);
};
