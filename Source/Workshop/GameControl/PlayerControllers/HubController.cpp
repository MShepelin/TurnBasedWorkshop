// Fill out your copyright notice in the Description page of Project Settings.


#include "HubController.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveCharacter.h"
#include "UObject/UObjectBaseUtility.h"
#include "Async/AsyncWork.h"
#include "Kismet/GameplayStatics.h"
#include "../ChoicesInstance.h"


AHubController::AHubController()
{
  
}

void AHubController::SetupInputComponent()
{
  Super::SetupInputComponent();

  InputComponent->BindAction("Interact", IE_Pressed, this, &AHubController::ChooseCharacter);

  InputComponent->BindAction("Apply", IE_Pressed, this, &AHubController::ApplyChosenCharacters);
}

void AHubController::ChooseCharacter()
{
  AActor* ChosenObject = GeneralRayCast();

  if (!ChosenObject)
  {
    return;
  }

  AInteractiveCharacter* InteractiveCharacter = Cast<AInteractiveCharacter>(ChosenObject);

  if (!InteractiveCharacter || ChosenCharacters.Find(InteractiveCharacter))
  {
    return;
  }

  ChosenCharacters.Add(InteractiveCharacter);
}

void AHubController::ApplyChosenCharacters()
{
  if (bStartedLevelLoading)
  {
    return;
  }
  else
  {
    bStartedLevelLoading = true;
  }

  TArray<TTuple<FCharacterCore, FInteractiveCore>>& ChosenCharactersInGame = Cast<UChoicesInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->ChosenCharacters;
  for (AInteractiveCharacter* ChosenCharacter : ChosenCharacters)
  {
    ChosenCharactersInGame.Add(TTuple<FCharacterCore,FInteractiveCore>(ChosenCharacter->CharacterDataCore, ChosenCharacter->InteractiveDataCore));
  }

  UE_LOG(LogTemp, Warning, TEXT("%d characters chosen"), ChosenCharacters.Num());
  
  LoadPackageAsync(
    FString("/Game/Experiments/FightMap"),
    FLoadPackageAsyncDelegate::CreateLambda([=](const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result)
    {
      if (Result == EAsyncLoadingResult::Succeeded)
      {
        ReadyForNextLevel();
      }
    }),
    TAsyncLoadPriority(0),
    EPackageFlags(1),
    0
  );
}

void AHubController::ReadyForNextLevel()
{
  UGameplayStatics::OpenLevel(this, FName("FightMap"), true);
}
