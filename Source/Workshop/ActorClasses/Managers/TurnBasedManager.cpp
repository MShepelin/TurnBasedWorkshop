// ...

#include "TurnBasedManager.h"
#include "Workshop/Types/Components/TurnBasedComponent.h"
#include "Components/TextRenderComponent.h"

void ATurnBasedManager::AddController(AController* NewController)
{
  UActorComponent* NeededComponent = NewController->GetComponentByClass(UTurnBasedComponent::StaticClass());
  if (!NeededComponent)
  {
    UE_LOG(LogTemp, Error, TEXT("Can't add controller without UTurnBasedComponent"));
    return;
  }

  UTurnBasedComponent* TurnBasedComponent = Cast< UTurnBasedComponent>(NeededComponent);

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

void ATurnBasedManager::OnConstruction(const FTransform & Transform)
{
  Super::OnConstruction(Transform);

#if WITH_EDITOR
  UInstancedStaticMeshComponent* SpawnLocations = NewObject<UInstancedStaticMeshComponent>(this, UInstancedStaticMeshComponent::StaticClass());
  SpawnLocations->SetStaticMesh(LocationMesh);
  SpawnLocations->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
  SpawnLocations->RegisterComponent();

  for (FCameraLocation ControllerLocation : ControllerLocations)
  {
    SpawnLocations->AddInstance(FTransform(FRotator(0, 90, 0), ControllerLocation.CameraSelfLocation, FVector(1, 1, 1)));

    for (FVector CharacterSpawnLocation : ControllerLocation.ParentLocations)
    {
      SpawnLocations->AddInstance(FTransform(FRotator(0, 90, 0), CharacterSpawnLocation, FVector(1, 1, 1)));
    }
  }
#endif
}
