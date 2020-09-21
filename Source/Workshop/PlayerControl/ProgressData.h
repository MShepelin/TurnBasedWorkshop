// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ProgressData.generated.h"


class AInteractiveCharacter;


UCLASS()
class WORKSHOP_API UProgressData : public USaveGame
{
	GENERATED_BODY()
	
public:
  UPROPERTY(VisibleAnywhere, Category = "BrigadeSetup")
  TArray<AInteractiveCharacter*> AvailableCharacters;

  UPROPERTY(VisibleAnywhere, Category = "BrigadeSetup")
  int32 TimeCounter;
};
