// ...

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveCharacter.h"
#include "ChoicesInstance.generated.h"


UCLASS()
class WORKSHOP_API UChoicesInstance : public UGameInstance
{
  GENERATED_BODY()

public:
  TArray<AInteractiveCharacter*> ChosenCharacters;
};
