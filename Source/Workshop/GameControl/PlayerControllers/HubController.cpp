// Copyright © Dmitriy Shepelin 2021. MIT License.


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
  if (CurrentCamera)
  {
    CurrentCamera->PlayerPressedClick();
  }

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

  UChoicesInstance* Choices = Cast<UChoicesInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
  for (AInteractiveCharacter* ChosenCharacter : ChosenCharacters)
  {
    Choices->ChosenCharacterClasses.Add(ChosenCharacter->GetClass());
    Choices->ChosenCharacterOptions.Add(ChosenCharacter->CharacterDataCore);
    Choices->ChosenInteractiveOptions.Add(ChosenCharacter->InteractiveDataCore);
  }

  UE_LOG(LogTemp, Warning, TEXT("%d characters chosen"), ChosenCharacters.Num());
  
  LoadPackageAsync(
    FString("/Game/Levels/FightMap"),
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
