// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EffectData.h"
#include "ChangeStatEffectData.generated.h"


UCLASS(Blueprintable, BlueprintType)
class WORKSHOP_API UChangeStatEffectData : public UEffectData
{
  GENERATED_BODY()

protected:
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings", meta = (OverrideNativeName = "ValueToUse"))
  int32 EffectValue = 0;

public:
  UChangeStatEffectData();
};