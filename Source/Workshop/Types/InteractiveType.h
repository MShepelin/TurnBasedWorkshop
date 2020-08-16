#pragma once

#include "CoreMinimal.h"
#include "InteractiveType.generated.h"


UENUM(BlueprintType)
enum EInteractiveType
{
  Base        UMETA(DisplayName = "Any"),
  Ability     UMETA(DisplayName = "Ability"),
  Character   UMETA(DisplayName = "Character"),
};
