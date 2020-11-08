//...

#include "CoreMinimal.h"
#include "Managers/TurnBasedManager.h"
#include "Workshop/GameControl/PlayerControllers/InteractController.h"
#include "CharacterSpawn.generated.h"

UCLASS()
class ACharacterSpawn : public AActor
{
  GENERATED_BODY()

public:
  // IDs will be sorted in the ascending order to arrange locations to spawn characters
  UPROPERTY(EditAnywhere) int32 CharacterOrderID = 0;
  UPROPERTY(EditAnywhere) bool bPlayerControlled = true;

public:
  //ACharacterSpawn();

  void PostInitializeComponents() override;

  void RegisterSpawnTransform();
};
