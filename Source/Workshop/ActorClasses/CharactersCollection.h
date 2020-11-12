//...

#include "CoreMinimal.h"
#include "Managers/TurnBasedManager.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveCharacter.h"
#include "CharactersCollection.generated.h"

UCLASS()
class ACharactersCollection : public APawn
{
  GENERATED_BODY()

public:
  UPROPERTY(EditInstanceOnly) TArray<TSubclassOf<AInteractiveCharacter>> CharacterClasses;
};
