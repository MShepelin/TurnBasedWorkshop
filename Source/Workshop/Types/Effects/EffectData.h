// Copyright © Dmitriy Shepelin 2021. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Workshop/ActorClasses/Managers/RegistrationManager.h"
#include "Workshop/WorkshopGameModeBase.h"
#include "EffectData.generated.h"

USTRUCT(BlueprintType)
struct WORKSHOP_API FEffectData
{
  GENERATED_BODY()

public:
  // Duration of effect:
  // -1 means that effect duration is infinite,
  // other values show how long effect will stay with object
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings", meta = (ClampMin = "-1"))
  int32 Duration = 0;

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings", meta = (OverrideNativeName = "ValueToUse"))
  int32 EffectValue = 0;

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings", meta = (OverrideNativeName = "StatIdentifier"))
  int32 StatID = 0;

  // If this is true effect is considered to be bonus, like it came from other ability,
  // so it's duration is decreased and it's overall considered as temporary effect.
  // If false effect is considered as initial effect of ability.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings")
  bool bIsBonusEffect = false;

  int32 DecreaseDuration(int32 OnValue);
  int32 DecreaseDuration();

  FString GetInfoString(AWorkshopGameModeBase* GameMode);
};
