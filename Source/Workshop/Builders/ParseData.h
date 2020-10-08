// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveAbility.h"
#include "Workshop/Types/Effects/AdvantageEffectData.h"
#include "Workshop/Types/Effects/ChangeStatEffectData.h"
#include "ParseData.generated.h"


// Functions from UBuildAbility should be added to CustomEffect 
// of AInteractiveAbility to implement needed gameplay.
UCLASS()
class WORKSHOP_API UParseData : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
  
public:
  // Parse information abot AccumulatedEffects.
  UFUNCTION(BlueprintCallable)
  FString GetAccumulatedEffects(AInteractiveObject* TargetObject);
};
