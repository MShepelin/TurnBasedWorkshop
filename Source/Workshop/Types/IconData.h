// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "IconData.generated.h"

USTRUCT(BlueprintType)
struct WORKSHOP_API FIconData
{
  GENERATED_BODY()

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "IconProperties")
  float Scale = 1.f;

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "IconProperties")
  UPaperFlipbook* SpriteIfAvailable;

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "IconProperties")
  UPaperFlipbook* SpriteIfUnavailable;
};
