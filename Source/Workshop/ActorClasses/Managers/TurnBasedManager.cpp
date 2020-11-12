// ...

#include "TurnBasedManager.h"
#include "Workshop/Types/Components/TurnBasedComponent.h"
#include "Components/TextRenderComponent.h"
#include "Workshop/GameControl/GameModes/FightGameMode.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveCharacter.h"

ATurnBasedManager::ATurnBasedManager()
{
  CurrentControllerIndex = 0;
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
    CurrentTurnPhase = ETurnPhase::Start;
    NextPhase();
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

  switch (CurrentTurnPhase)
  {
  case ETurnPhase::Start:
  {
    JoinedControllers[CurrentControllerIndex]->TurnIsTakenUnderControl.ExecuteIfBound();
    break;
  }
  case ETurnPhase::End:
  {
    JoinedControllers[CurrentControllerIndex]->TurnIsOutOfControl.ExecuteIfBound();

    // Change current controller
    CurrentControllerIndex++;
    if (CurrentControllerIndex == JoinedControllers.Num())
    {
      CurrentControllerIndex = 0;
    }
    break;
  }

  default:
    break;
  }

  if (CurrentTurnPhase == ETurnPhase::End)
  {
    CurrentTurnPhase = ETurnPhase::Start;
  }
  else
  {
    CurrentTurnPhase = static_cast<ETurnPhase>(static_cast<uint8>(CurrentTurnPhase) + 1);
  }
}

void ATurnBasedManager::SpawnCharacters()
{
  /*
  EnemySpawnLocations.Sort(
    [](const TPair<int32, FTransform>& Left, const TPair<int32, FTransform>& Right) { return Left.Key < Right.Key; }
  );

  FActorSpawnParameters SpawnParams = FActorSpawnParameters();
  SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

  for (TSubclassOf<AInteractiveCharacter> SpawnClass : EnemyClasses)
  {
    AInteractiveCharacter* NewCharacter = GetWorld()->SpawnActor<AInteractiveCharacter>(SpawnClass, SpawnParams);
    NewCharacter->RefreshInteractive();
    Enemies.Add(NewCharacter);
  }

  int32 MaxLocations = EnemySpawnLocations.Num();
  check(MaxLocations >= Enemies.Num());
  FTransform CameraSpawnTransform;
  int32 LocationCounter = (MaxLocations - Enemies.Num()) / 2;
  for (AInteractiveCharacter* PlacableCharacter : Enemies)
  {
    CameraSpawnTransform = EnemySpawnLocations[LocationCounter].Value;
    PlacableCharacter->SetActorTransform(CameraSpawnTransform);
    LocationCounter++;

    ConnectObject(PlacableCharacter);
  }*/
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
