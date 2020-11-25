// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveAbility.h"
#include "Workshop/Types/Effects/ChangeStatEffectData.h"
#include "BuildAbility.generated.h"


// Functions from UBuildAbility should be added to CustomEffect 
// of AInteractiveAbility to implement needed gameplay.
UCLASS()
class WORKSHOP_API UBuildAbility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
  
public:
  // --------------------- //
  // General manipulations //
  // --------------------- //

  // Adds all effects of ability without checking if ability is applicable.
  UFUNCTION(BlueprintCallable)
  static AInteractiveObject* AddAllEffectsToObject(AInteractiveObject* TargetObject, AInteractiveAbility* Ability);
};
