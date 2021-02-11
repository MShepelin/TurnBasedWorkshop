// ...

#include "FightAI.h"
#include "Workshop/GameControl/GameModes/FightGameMode.h"

AFightAI::AFightAI()
{

}

void AFightAI::OnGetTurnControl_Implementation()
{
  UE_LOG(LogTemp, Warning, TEXT("AI gained control and passed it!"));

  while (UsedManager->GetPhase() != ETurnPhase::End)
  {
    UsedManager->NextPhase();
  }

  UsedManager->NextPhase();
}

void AFightAI::OnConnectToManager_Implementation()
{
  if (!GetPawn())
  {
    UE_LOG(LogTemp, Error, TEXT("Controller must possess a TurnBasedObserver to spawn characters"));
    return;
  }

  ATurnBasedObserver* PossessedObserver = Cast<ATurnBasedObserver>(GetPawn());
  if (!PossessedObserver)
  {
    UE_LOG(LogTemp, Error, TEXT("Possessed pawn must inherit from TurnBasedObserver"));
    return;
  }

  PossessedObserver->SpawnCharacters();
  for (AInteractiveCharacter* SpawnedCharacter : PossessedObserver->GetSpawnedCharacters())
  {
    uint8 TypeMask = SpawnedCharacter->GetInteractiveType();
    TypeMask |= static_cast<uint8>(EInteractiveType::NotPlayerControlled);
    TypeMask &= ~static_cast<uint8>(EInteractiveType::PlayerControlled);
    SpawnedCharacter->SetInteractiveType(static_cast<EInteractiveType>(TypeMask));
  }

  PossessedObserver->ConnectAllSpawnedObjects();
  UsedManager = PossessedObserver->GetManager();
}
