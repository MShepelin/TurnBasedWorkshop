// ...

#include "TurnBasedManager.h"
#include "Workshop/Types/Components/TurnBasedComponent.h"
#include "Components/TextRenderComponent.h"
#include "Workshop/GameControl/GameModes/FightGameMode.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveCharacter.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveAbility.h"
#include "Workshop/ActorClasses/CharacterSpawn.h"
#include "TurnBasedObserver.h"

ATurnBasedManager::ATurnBasedManager()
{
  
}

void ATurnBasedManager::AddTurnBasedController(ATurnBasedObserver* NewController)
{
  JoinedControllers.Add(NewController);
  NewController->OnConnectToManager(this);

  // Initialise turn if needed
  if (JoinedControllers.Num() == 1)
  {
    CurrentControllerIndex = 0;
    CurrentTurnPhase = ETurnPhase::Start;
    JoinedControllers[CurrentControllerIndex]->OnGetTurnControl();
  }
}

void ATurnBasedManager::RemoveTurnBasedController(ATurnBasedObserver* RemovedController)
{
  int32 FoundIndex = JoinedControllers.Find(RemovedController);
  if (FoundIndex == INDEX_NONE)
  {
    UE_LOG(LogTemp, Error, TEXT("The observer isn't connected with this Manager"));
    return;
  }

  if (FoundIndex > CurrentControllerIndex)
  {
    --CurrentControllerIndex;
  }

  if (FoundIndex == CurrentControllerIndex)
  {
    CurrentTurnPhase = ETurnPhase::Start;
    JoinedControllers[CurrentControllerIndex]->OnGetTurnControl();
  }

  JoinedControllers.RemoveAt(FoundIndex);
  RemovedController->OnLoseTurnControl();
}

ETurnPhase ATurnBasedManager::GetPhase() const
{
  return CurrentTurnPhase;
}

void ATurnBasedManager::NextPhase()
{
  // It's guaranteed that JoinedControllers doesn't have null-pointers.

  if (!JoinedControllers.Num())
  {
    UE_LOG(LogTemp, Warning, TEXT("Can't go to the next phase without controllers"));
    return;
  }

  if (CurrentTurnPhase == ETurnPhase::End)
  {
    CurrentTurnPhase = ETurnPhase::Start;
  }
  else
  {
    CurrentTurnPhase = static_cast<ETurnPhase>(static_cast<uint8>(CurrentTurnPhase) + 1);
  }

  if (CurrentTurnPhase == ETurnPhase::Start)
  {
    // Change current controller
    JoinedControllers[CurrentControllerIndex]->OnLoseTurnControl();
    
    CurrentControllerIndex++;
    if (CurrentControllerIndex == JoinedControllers.Num())
    {
      CurrentControllerIndex = 0; 
    }

    JoinedControllers[CurrentControllerIndex]->OnGetTurnControl();
  }
}

const TArray<ACharacterSpawn*>& ATurnBasedManager::GetCharacterSpawns(int32 TurnBasedID) const
{
  check(SpawnLocations.Contains(TurnBasedID));
  return SpawnLocations[TurnBasedID].CharacterSpawns;
}
