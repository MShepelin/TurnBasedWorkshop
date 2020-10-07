// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Workshop/ActorClasses/InteractiveObjects/InteractiveAbility.h"
#include "ChangeStatEffectData.generated.h"


UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class WORKSHOP_API UChangeStatEffectData : public UEffectData
{
  GENERATED_BODY()

public:
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings", meta = (OverrideNativeName = "ValueToUse"))
  int32 EffectValue = 0;

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings", meta = (OverrideNativeName = "StatIdentifier"))
  int32 StatID = 0;

  UChangeStatEffectData();

  virtual void ResolveOn(AInteractiveObject* TargetObject) override;
};
