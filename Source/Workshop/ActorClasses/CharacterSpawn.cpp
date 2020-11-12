// ...

#include "CharacterSpawn.h"
#include "Workshop/ActorClasses/Managers/TurnBasedManager.h"
#include "Workshop/GameControl/GameModes/FightGameMode.h"
#include "Workshop/GameControl/AI/FightController.h"
#include "Kismet/GameplayStatics.h"

void ACharacterSpawn::PostInitializeComponents()
{
  Super::PostInitializeComponents();

  if (!GetWorld())
  {
    return;
  }

  AFightGameMode* GameMode = Cast<AFightGameMode>(GetWorld()->GetAuthGameMode());
  if (!GameMode)
  {
    return;
  }
  GameMode->RegisterAllSpawnLocations.AddUObject(this, &ACharacterSpawn::RegisterSpawnTransform);
}

void ACharacterSpawn::RegisterSpawnTransform()
{
  if (ControllerID == -1)
  {
    AInteractController* Controller = Cast<AInteractController>(
      UGameplayStatics::GetPlayerController(GetWorld(), 0));
    check(Controller);

    Controller->AddSpawnTransform(GetActorTransform(), CharacterOrderID);
    return;
  }

  AFightGameMode* GameMode = Cast<AFightGameMode>(GetWorld()->GetAuthGameMode());
  if (GameMode && ControllerID < GameMode->FightControllers.Num())
  {
    UE_LOG(LogTemp, Warning, TEXT("Connection happened"));
    GameMode->FightControllers[ControllerID].Value->CharactersSpawnTransforms.Add(
      TPairInitializer<int32, FTransform>(CharacterOrderID, GetActorTransform()));
  }
}
