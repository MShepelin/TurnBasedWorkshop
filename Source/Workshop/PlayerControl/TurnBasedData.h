// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TurnBasedData.generated.h"


class AInteractiveCharacter;


UCLASS()
class WORKSHOP_API UTurnBasedData : public USaveGame
{
	GENERATED_BODY()
	
public:
  UPROPERTY(VisibleAnywhere, Category = "BrigadeSetup")
  TArray<AInteractiveCharacter*> ChosenCharacters;

  UPROPERTY(VisibleAnywhere, Category = "BrigadeSetup")
  int32 TestIntValue;
};
