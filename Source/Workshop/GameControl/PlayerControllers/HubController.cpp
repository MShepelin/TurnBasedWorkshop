// Fill out your copyright notice in the Description page of Project Settings.


#include "HubController.h"
#include "Kismet/GameplayStatics.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveCharacter.h"
#include "../ChoicesInstance.h"


AHubController::AHubController()
{
  
}

void AHubController::SetupInputComponent()
{
  Super::SetupInputComponent();

  InputComponent->BindAction("Interact", IE_Pressed, this, &AHubController::ChooseCharacter);
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
  Cast<UChoicesInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->ChosenCharacters = ChosenCharacters.Array();
}
