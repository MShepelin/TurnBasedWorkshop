// ...

#include "CharacterSpawn.h"
#include "Workshop/ActorClasses/Managers/TurnBasedManager.h"
#include "Workshop/GameControl/GameModes/FightGameMode.h"
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
  AInteractController* Controller = Cast<AInteractController>(
    UGameplayStatics::GetPlayerController(GetWorld(), 0));
  check(Controller);

  if (bPlayerControlled)
  {
    Controller->AddSpawnTransform(GetActorTransform(), CharacterOrderID);
    return;
  }
  
  ATurnBasedManager* Manager = Cast<ATurnBasedManager>(Controller->GetUsedManager());
  if (Manager)
  {
    Manager->EnemySpawnLocations.Add(TPairInitializer<int32, FTransform>(CharacterOrderID, GetActorTransform()));
  }
}
