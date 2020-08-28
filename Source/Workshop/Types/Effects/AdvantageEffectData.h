// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EffectData.h"
#include "Workshop/Types/AdvantageType.h"
#include "AdvantageEffectData.generated.h"


UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class WORKSHOP_API UAdvantageEffectData : public UEffectData
{
  GENERATED_BODY()

public:
  // Type of Advantage to use.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings")
  EAdvantageType AdvantageType;

  // Types of objects who is Advantage against. Affects everyone if empty.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings")
  TArray<int32> AdvantageCTs;

  UAdvantageEffectData();

  virtual FString GatherInformation(bool bIsAbilityInfo, ARegistrationManager* Manager) const override;
};
