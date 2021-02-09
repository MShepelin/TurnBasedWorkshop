// Fill out your copyright notice in the Description page of Project Settings.

#include "TurnBasedObserver.h"
#include "Workshop/ActorClasses/CharacterSpawn.h"
#include "Workshop/Interfaces/TurnBasedInterface.h"

void ATurnBasedObserver::OnConnectToManager(ATurnBasedManager *Manager)
{
  TurnBasedManager = Manager;

  if (TurnBasedController.GetInterface())
  {
    TurnBasedController->Execute_OnConnectToManager(TurnBasedController.GetObject());
  }
}

void ATurnBasedObserver::OnDisconnectFromManager()
{
  if (!TurnBasedManager)
  {
    return;
  }

  if (TurnBasedController.GetInterface())
  {
    TurnBasedController->Execute_OnDisconnectFromManager(TurnBasedController.GetObject());
  }

  TurnBasedManager = nullptr;
}

void ATurnBasedObserver::ConnectAllSpawnedObjects()
{
  for (AInteractiveCharacter* SpawnedCharacter : SpawnedCharacters)
  {
    TurnBasedManager->ConnectObject(SpawnedCharacter);
  }
}

void ATurnBasedObserver::OnGetTurnControl()
{
  if (TurnBasedController.GetInterface())
  {
    TurnBasedController->Execute_OnGetTurnControl(TurnBasedController.GetObject());
  }
}

void ATurnBasedObserver::OnLoseTurnControl()
{
  if (TurnBasedController.GetInterface())
  {
    TurnBasedController->Execute_OnLoseTurnControl(TurnBasedController.GetObject());
  }
}

void ATurnBasedObserver::PossessedBy(AController *NewController)
{
  Super::PossessedBy(NewController);

  TurnBasedController.SetObject(NewController);
  TurnBasedController.SetInterface(Cast<ITurnBasedInterface>(NewController));
}

void ATurnBasedObserver::UnPossessed()
{
  Super::UnPossessed();

  TurnBasedController.SetObject(nullptr);
  TurnBasedController.SetInterface(nullptr);
}

void ATurnBasedObserver::SpawnCharacters()
{
  if (!TurnBasedManager)
  {
    UE_LOG(LogTemp, Error, TEXT("Can't spawn without manager"));
    return;
  }

  if (SpawnedCharacters.Num())
  {
    UE_LOG(LogTemp, Error, TEXT("Before spawning remove characters that were spawned before"));
    return;
  }

  const TArray<ACharacterSpawn*> &CharacterSpawns = TurnBasedManager->GetCharacterSpawns(TurnBasedID);

  int32 MaxLocations = CharacterSpawns.Num();
  if (MaxLocations < CharactersToUse.Num())
  {
    UE_LOG(LogTemp, Error, TEXT("Not enough spawn places for observer with turn-based ID %d"), TurnBasedID);
    return;
  }

  // Spawn characters
  FActorSpawnParameters SpawnParams = FActorSpawnParameters();
  SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
  for (int32 CharacterIndex = 0; CharacterIndex < CharactersToUse.Num(); ++CharacterIndex)
  {
    AInteractiveCharacter* NewCharacter = GetWorld()->SpawnActor<AInteractiveCharacter>(CharactersToUse[CharacterIndex], SpawnParams);
    NewCharacter->BuildInteractive();
    SpawnedCharacters.Add(NewCharacter);
  }

  // Additional options
  if (CharactersOptions.Num())
  {
    for (int32 CharacterIndex = 0; CharacterIndex < SpawnedCharacters.Num(); ++CharacterIndex)
    {
      // The only way to set CharactersOptions and InteractiveOptions is to use SetCharactersToUse
      // which ensures that the sizes of arrays will be correct so CharacterIndex can be used without checks
      SpawnedCharacters[CharacterIndex]->CharacterDataCore = CharactersOptions[CharacterIndex];
      SpawnedCharacters[CharacterIndex]->InteractiveDataCore = InteractiveOptions[CharacterIndex];
    }
  }

  // Give characters appropriate transforms
  FTransform CameraSpawnTransoform;
  int32 LocationCounter = (MaxLocations - SpawnedCharacters.Num()) / 2;
  for (AInteractiveCharacter* PlacableCharacter : SpawnedCharacters)
  {
    CameraSpawnTransoform = CharacterSpawns[LocationCounter]->GetActorTransform();
    // Or get some additional spawn options
    PlacableCharacter->SetActorTransform(CameraSpawnTransoform);
    LocationCounter++;
  }
}

void ATurnBasedObserver::RemoveCharacters()
{
  for (AInteractiveCharacter* PlacableCharacter : SpawnedCharacters)
  {
    PlacableCharacter->Destroy(true, true);
  }

  SpawnedCharacters.Empty();
}

ATurnBasedManager* ATurnBasedObserver::GetManager()
{
  return TurnBasedManager;
}

void ATurnBasedObserver::SetCharactersToUse(
  const TArray<TSubclassOf<AInteractiveCharacter>>& NewCharacterClasses,
  const TArray<FCharacterCore> *NewCharactersOptions,
  const TArray<FInteractiveCore> *NewInteractiveOptions
)
{
  CharactersToUse = NewCharacterClasses;
  if (!(NewCharactersOptions && NewInteractiveOptions))
  {
    return;
  }

  CharactersOptions.Empty();
  InteractiveOptions.Empty();

  if (NewCharactersOptions->Num() != NewInteractiveOptions->Num() || NewInteractiveOptions->Num() != NewCharacterClasses.Num())
  {
    UE_LOG(LogTemp, Warning, TEXT("Options were passed, but their shapes didn't match, it's probably a mistake"));
    return;
  }

  CharactersOptions = *NewCharactersOptions;
  InteractiveOptions = *NewInteractiveOptions;
}

void ATurnBasedObserver::SetTurnBasedID(int32 NewID)
{
  TurnBasedID = NewID;
}

int32 ATurnBasedObserver::GetTurnBasedID()
{
  return TurnBasedID;
}

TArray<AInteractiveCharacter*>& ATurnBasedObserver::GetSpawnedCharactes()
{
  return SpawnedCharacters;
}
