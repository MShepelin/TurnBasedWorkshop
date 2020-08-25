// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EffectData.h"
#include "ChangeStatEffectData.generated.h"


UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class WORKSHOP_API UChangeStatEffectData : public UEffectData
{
  GENERATED_BODY()

protected:
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings", meta = (OverrideNativeName = "ValueToUse"))
  int32 EffectValue;

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings", meta = (OverrideNativeName = "StatIdentifier"))
  int32 StatID;

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings")
  bool bIsRelative;

public:
  UChangeStatEffectData();

  virtual FString GatherInformation(bool bIsAbilityInfo, ARegistrationManager* Manager) const override;
};