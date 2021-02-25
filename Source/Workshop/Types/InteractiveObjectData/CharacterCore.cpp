// Copyright © Dmitriy Shepelin 2021. MIT License.

#include "CharacterCore.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveCharacter.h"

FCharacterCore& FCharacterCore::operator=(const FCharacterCore& AnotherCore)
{
  AbilitiesClasses = AnotherCore.AbilitiesClasses;
  AnimationsMap = AnotherCore.AnimationsMap;
  bIsExhausted.AtomicSet(AnotherCore.bIsExhausted);
  return *this;
}

FCharacterCore::FCharacterCore()
{
  bIsExhausted = false;
}

FCharacterCore::FCharacterCore(const FCharacterCore& AnotherCore)
{
  AbilitiesClasses = AnotherCore.AbilitiesClasses;
  AnimationsMap = AnotherCore.AnimationsMap;
  bIsExhausted.AtomicSet(AnotherCore.bIsExhausted);
}
