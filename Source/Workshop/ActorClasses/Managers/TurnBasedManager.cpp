// ...

#include "TurnBasedManager.h"
#include "Workshop/Types/Components/TurnBasedComponent.h"
#include "Components/TextRenderComponent.h"


ATurnBasedManager::ATurnBasedManager()
{
  SpawnLocations = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ControllerLocations"));
  SpawnLocations->SetupAttachment(RootComponent);
  SpawnLocations->SetHiddenInGame(true, true);

  AddedControllers = 0;
}

void ATurnBasedManager::AddController(AController* NewController)
{
  UActorComponent* NeededComponent = NewController->GetComponentByClass(UTurnBasedComponent::StaticClass());
  if (!NeededComponent)
  {
    UE_LOG(LogTemp, Error, TEXT("Can't add controller without UTurnBasedComponent"));
    return;
  }

  if (AddedControllers == SpawnLocations->GetInstanceCount())
  {
    UE_LOG(LogTemp, Error, TEXT("Not enough places to add new controller."));
    return;
  }

  NewController->GetPawn()->SetActorTransform(ControllersLocations[AddedControllers]);
  AddedControllers++;

  UTurnBasedComponent* TurnBasedComponent = Cast<UTurnBasedComponent>(NeededComponent);
  TurnBasedComponent->Manager = this;
  TurnBasedComponent->bIsTurnControlled = JoinedControllers.Num() ? false : true;
  JoinedControllers.Add(TurnBasedComponent);
  TurnBasedComponent->ConnectionHappened();
}

void ATurnBasedManager::RemoveController(AController* NewController)
{
  UActorComponent* NeededComponent = NewController->GetComponentByClass(UTurnBasedComponent::StaticClass());
  if (!NeededComponent)
  {
    UE_LOG(LogTemp, Error, TEXT("Can't remove controller without UTurnBasedComponent"));
    return;
  }

  UTurnBasedComponent* TurnBasedComponent = Cast<UTurnBasedComponent>(NeededComponent);

  if (JoinedControllers.Find(TurnBasedComponent) == INDEX_NONE)
  {
    return;
  }

  TurnBasedComponent->Manager = nullptr;
  JoinedControllers.Remove(TurnBasedComponent);
  JoinedControllers.Shrink();
}

ETurnPhase ATurnBasedManager::GetPhase() const
{
  return CurrentTurnPhase;
}

void ATurnBasedManager::NextPhase()
{
  if (!JoinedControllers.Num())
  {
    UE_LOG(LogTemp, Warning, TEXT("Can't go to the next phase without controllers"));
    return;
  }

  switch (CurrentTurnPhase)
  {
  case ETurnPhase::Start:
    if (CurrentControllerIndex == JoinedControllers.Num())
    {
      CurrentControllerIndex = 0;
    }

    //???? check for null before that
    JoinedControllers[CurrentControllerIndex]->bIsTurnControlled = true;
    break;

  case ETurnPhase::End:
    JoinedControllers[CurrentControllerIndex]->bIsTurnControlled = false;
    //JoinedControllers[CurrentControllerIndex]->
    CurrentControllerIndex++;
    break;

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

void ATurnBasedManager::PostInitializeComponents()
{
  Super::PostInitializeComponents();

  ControllersLocations.Init(FTransform(), SpawnLocations->GetInstanceCount());
  for (int32 Index = 0; Index < SpawnLocations->GetInstanceCount(); Index++)
  {
    SpawnLocations->GetInstanceTransform(Index, ControllersLocations[Index], true);
  }
}
