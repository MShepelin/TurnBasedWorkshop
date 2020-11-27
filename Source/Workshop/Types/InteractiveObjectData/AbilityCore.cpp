//...

#include "AbilityCore.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveAbility.h"

FAbilityCore& FAbilityCore::operator=(const FAbilityCore& AnotherCore)
{
  AbilityAnimationId = AnotherCore.AbilityAnimationId;
  UsedEffects = AnotherCore.UsedEffects;
  CTsToAffect = AnotherCore.CTsToAffect;
  TargetTypeMask = AnotherCore.TargetTypeMask;
  NumOfTargets = AnotherCore.NumOfTargets;
  IconUI = AnotherCore.IconUI;
  IconScene = AnotherCore.IconScene;

  return *this;
}
