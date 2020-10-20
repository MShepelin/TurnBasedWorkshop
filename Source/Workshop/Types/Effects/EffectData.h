// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include <algorithm>
#include "CoreMinimal.h"
#include "Workshop/ActorClasses/Managers/RegistrationManager.h"
#include "Workshop/Types/InteractiveType.h"
#include "Workshop/Types/TurnPhase.h"
#include "EffectData.generated.h"


UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class WORKSHOP_API UEffectData : public UObject
{
  GENERATED_BODY()

public:
  // Duration of effect:
  // 0 means that effect is applied when added, 
  // -1 means that effect duration is infinite,
  // other values show how long effect will stay with object
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings", meta = (ClampMin = "-1"))
  int32 Duration = 0;

  // If this is true effect is considered to be bonus, like it came from other ability,
  // so it's duration is decreased and it's overall considered as temporary effect.
  // If false effect is considered as initial effect of ability.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings")
  bool bIsBonusEffect = false;

  // If Effect is ambigous, it will pass from ability to ability as bonus effect.
  // Example: Health +10 can is ambiguous beacause it can be both an effect on a character and
  // bonus effect to another ability.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings")
  bool bIsAmbiguous = false;

  // If true this effect will affect target regardless of existing target stats.
  // Example: Fire Resistance -10 can be applicable even if target object doesn't have Fire Resistance stat
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings")
  bool bIsForciblyAdded = false;

  //++++ make flag mask from all bool variables
  //++++ add visualisation

  // Used when duration is nonzero to choose when to resolve effect on object.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EffectSettings")
  ETurnPhase TurnPhaseToResolve = ETurnPhase::Start;

  UEffectData();

  void DecreaseDuration(int32 OnValue);
  void DecreaseDuration();

  virtual FString GetInfoString(ARegistrationManager* UsedManager);

  // REMAKE
  //virtual FString GatherInformation(bool bIsAbilityInfo, ARegistrationManager* Manager) const;
  virtual void ResolveOn(AInteractiveObject* TargetObject);
};
