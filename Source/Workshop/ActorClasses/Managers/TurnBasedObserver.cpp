// Fill out your copyright notice in the Description page of Project Settings.

#include "TurnBasedObserver.h"
#include "Workshop/Interfaces/TurnBasedInterface.h"

void ATurnBasedObserver::OnConnectToManager(ATurnBasedManager *Manager)
{
  TurnBasedManager = Manager;

  if (TurnBasedController.GetInterface())
  {
    TurnBasedController->OnConnectToManager();
  }
}

void ATurnBasedObserver::OnGetTurnControl()
{

}

void ATurnBasedObserver::OnLoseTurnControl()
{

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

void ATurnBasedObserver::SpawnCharacters(FVector HiddenLocation)
{

}

void ATurnBasedObserver::RemoveCharacters()
{

}

void ATurnBasedObserver::OnDisconnectFromManager()
{
  TurnBasedManager = nullptr;
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

  if (NewCharactersOptions->Num() != NewInteractiveOptions->Num() || NewInteractiveOptions->Num() != NewCharacterClasses.Num())
  {
    UE_LOG(LogTemp, Warning, TEXT("Options were passed, but their shapes didn't match, it's probably a mistake"));
    return;
  }

  CharactersOptions = *NewCharactersOptions;
  InteractiveOptions = *NewInteractiveOptions;
}
