//...

#include "CharacterCore.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveCharacter.h"

FCharacterCore::FCharacterCore()
{
  CharacterStats.Init({ {0, 1} }, CharacterIntegerStats);
}
