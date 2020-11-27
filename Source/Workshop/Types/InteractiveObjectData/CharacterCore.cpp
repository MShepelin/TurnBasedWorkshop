//...

#include "CharacterCore.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveCharacter.h"

FCharacterCore& FCharacterCore::operator=(const FCharacterCore& AnotherCore)
{
  AbilitiesClasses = AnotherCore.AbilitiesClasses;
  AnimationsMap = AnotherCore.AnimationsMap;
  return *this;
}
