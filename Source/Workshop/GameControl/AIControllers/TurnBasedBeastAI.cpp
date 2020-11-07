// ....

#include "TurnBasedBeastAI.h"

ATurnBasedBeastAI::ATurnBasedBeastAI()
{
  TurnControl = CreateDefaultSubobject<UTurnBasedComponent>(TEXT("TurnControl"));
  AddOwnedComponent(TurnControl);
  TurnControl->ConnectDelegate.BindUObject(this, &AInteractController::ConnectionHappened);
}

const ARegistrationManager* ATurnBasedBeastAI::GetUsedManager() const
{
  return UsedManager;
}

void ATurnBasedBeastAI::ConnectionHappened()
{
  ATurnBasedManager* EventManager = Cast<ATurnBasedManager>(UsedManager);
  // Disconnect from previous manager
  if (EventManager)
  {
    EventManager->RemoveController(this);
  }

  EventManager = TurnControl->GetManager();

  FActorSpawnParameters SpawnParams = FActorSpawnParameters();
  SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

  for (TSubclassOf<AInteractiveCharacter>>& CharacterToSpawn : UsedCharacters)
  {
    AInteractiveCharacter* NewCharacter = GetWorld()->SpawnActor<AInteractiveCharacter>(CharacterToSpawn, SpawnParams);
    //NewCharacter->SetActorLocation( get pawn -> get locations, spawn them );
    NewCharacter->RefreshInteractive();
    PlacableCharacters.Add(NewCharacter);
  }

  /*
  int32 MaxLocations = GetCurrentCamera()->SpawnLocations->GetInstanceCount();
  check(MaxLocations >= PlacableCharacters.Num());
  FTransform CameraSpawnTransoform;
  int32 LocationCounter = MaxLocations / 2 - PlacableCharacters.Num() / 2;
  for (AInteractiveCharacter* PlacableCharacter : PlacableCharacters)
  {
    GetCurrentCamera()->SpawnLocations->GetInstanceTransform(LocationCounter, CameraSpawnTransoform, true);
    PlacableCharacter->SetActorTransform(CameraSpawnTransoform);
    LocationCounter++;

    EventManager->ConnectObject(PlacableCharacter);
  }*/

  UsedManager = EventManager;
}
