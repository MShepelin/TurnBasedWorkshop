// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TurnBasedCamera.generated.h"


class AInteractiveCharacter;


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTurnBasedCamera : public UInterface
{
	GENERATED_BODY()
};


class WORKSHOP_API ITurnBasedCamera
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

  UFUNCTION(BlueprintNativeEvent)
  void OnCharacterResolvesAbility(AInteractiveCharacter* CurrentCharacter);

  UFUNCTION(BlueprintNativeEvent)
  FVector GetHiddenAbilitiesPosition() const;
};
