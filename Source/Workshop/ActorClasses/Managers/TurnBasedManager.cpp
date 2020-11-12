// ...

#include "TurnBasedManager.h"
#include "Workshop/Types/Components/TurnBasedComponent.h"
#include "Components/TextRenderComponent.h"
#include "Workshop/GameControl/GameModes/FightGameMode.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveCharacter.h"

ATurnBasedManager::ATurnBasedManager()
{
  
}

void ATurnBasedManager::AddController(AController* NewController)
{
  UActorComponent* NeededComponent = NewController->GetComponentByClass(UTurnBasedComponent::StaticClass());
  if (!NeededComponent)
  {
    UE_LOG(LogTemp, Error, TEXT("Can't add controller without UTurnBasedComponent"));
    return;
  }

  UTurnBasedComponent* TurnBasedComponent = Cast<UTurnBasedComponent>(NeededComponent);
  JoinedControllers.Add(TurnBasedComponent);
  TurnBasedComponent->ConnectDelegate.ExecuteIfBound();

  // Initialise turn if needed
  if (JoinedControllers.Num() == 1)
  {
    CurrentControllerIndex = 0;
    CurrentTurnPhase = ETurnPhase::Start;
    JoinedControllers[CurrentControllerIndex]->TurnIsTakenUnderControl.ExecuteIfBound();
  }
}

void ATurnBasedManager::RemoveController(AController* NewController)
{
  // REWORK needed as objects of InteractController don't get removed
  UActorComponent* NeededComponent = NewController->GetComponentByClass(UTurnBasedComponent::StaticClass());
  if (!NeededComponent)
  {
    UE_LOG(LogTemp, Error, TEXT("Can't remove controller without UTurnBasedComponent"));
    return;
  }

  UTurnBasedComponent* TurnBasedComponent = Cast<UTurnBasedComponent>(NeededComponent);

  if (JoinedControllers.Find(TurnBasedComponent) == INDEX_NONE)
  {
    UE_LOG(LogTemp, Error, TEXT("UTurnBasedComponent wasn't connected to this Manager"));
    return;
  }

  JoinedControllers.Remove(TurnBasedComponent);
  JoinedControllers.Shrink();
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
    UE_LOG(LogTemp, Warning, TEXT("Can't go to the next phase without external controllers"));
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

  switch (CurrentTurnPhase)
  {
  case ETurnPhase::Start:
  {
    // Change current controller
    JoinedControllers[CurrentControllerIndex]->TurnIsOutOfControl.ExecuteIfBound();
    
    CurrentControllerIndex++;
    if (CurrentControllerIndex == JoinedControllers.Num())
    {
      CurrentControllerIndex = 0;
    }

    JoinedControllers[CurrentControllerIndex]->TurnIsTakenUnderControl.ExecuteIfBound();
    break;
  }
  default:
    break;
  }
}

void ATurnBasedManager::MakeObjectsReady()
{
  AFightGameMode* GameMode = Cast<AFightGameMode>(GetWorld()->GetAuthGameMode());
  check(GameMode);

  GameMode->FightManager = this;
  GameMode->FightControllers.Sort(
    [](const TPair<int32, AFightController*>& Left, const TPair<int32, AFightController*>& Right) { return Left.Key < Right.Key; });
  GameMode->RegisterAllSpawnLocations.Broadcast();
  GameMode->ObjectsReady.Broadcast();
}
