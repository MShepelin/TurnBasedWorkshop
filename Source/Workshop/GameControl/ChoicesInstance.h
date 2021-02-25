// Copyright � Dmitriy Shepelin 2021. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Workshop/Types/InteractiveObjectData/CharacterCore.h"
#include "Workshop/Types/InteractiveObjectData/InteractiveCore.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveCharacter.h"
#include "Templates/SharedPointer.h"
#include "Containers/Map.h"
#include "ChoicesInstance.generated.h"

UCLASS()
class WORKSHOP_API UChoicesInstance : public UGameInstance
{
  GENERATED_BODY()

public:
  TArray<TSubclassOf<AInteractiveCharacter>> ChosenCharacterClasses;
  TArray<FCharacterCore> ChosenCharacterOptions;
  TArray<FInteractiveCore> ChosenInteractiveOptions;
};
