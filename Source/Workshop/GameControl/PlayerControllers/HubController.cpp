// Fill out your copyright notice in the Description page of Project Settings.


#include "HubController.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveCharacter.h"
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

  UE_LOG(LogTemp, Warning, TEXT("New character added!"));
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

  //Cast<UChoicesInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->ChosenCharacters = ChosenCharacters.Array();

  /*
  for (AInteractiveCharacter* ChosenCharacter : ChosenCharacters)
  {
    ChosenCharacter->AddToRoot();
    GetWorld()->RemoveActor(ChosenCharacter, true);
  }*/

  LoadPackageAsync(
    FString("/Game/Experiments/FightMap"),    //nullptr,//PendingTravelGuid.IsValid() ? &PendingTravelGuid : NULL,
    //NULL,
    //FLoadPackageAsyncDelegate::CreateRaw(this, &AHubController::ReadyForNextLevel),
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
