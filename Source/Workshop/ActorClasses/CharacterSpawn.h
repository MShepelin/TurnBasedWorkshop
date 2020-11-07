//...

#include "CoreMinimal.h"
#include "CharacterSpawn.generated.h"

UCLASS()
class ACharacterSpawn : public AActor
{
  GENERATED_BODY()

public:
  // IDs will be sorted in the ascending order to arrange locations to spawn characters
  int32 CharacterOrderID = 0;
  bool bPlayerControlled = false;
};
