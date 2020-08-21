// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveAbility.h"
#include "BuildAbility.generated.h"

/**
 * Functions from UBuildAbility should be added to construction script and CustomEffect 
 * of AInteractiveAbility to implement needed skills
 */
UCLASS()
class WORKSHOP_API UBuildAbility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
  UFUNCTION(BlueprintCallable)
  AInteractiveObject* AddEffectToObject(AInteractiveObject* TargetObject, AInteractiveAbility* Ability, int32 EffectIndex);
};
