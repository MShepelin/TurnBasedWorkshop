// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractController.h"
#include "Workshop/ActorClasses/Managers/TurnBasedManager.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveObject.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveCharacter.h"
#include "Workshop/ActorClasses/CameraWork/SpryCamera.h"
#include "Kismet/GameplayStatics.h"
#include "../ChoicesInstance.h"


AInteractController::AInteractController()
{
  TurnControl = CreateDefaultSubobject<UTurnBasedComponent>(TEXT("TurnControl"));
  AddOwnedComponent(TurnControl);
  TurnControl->ConnectDelegate.BindUObject(this, &AInteractController::ConnectionHappened);
}

void AInteractController::ConnectionHappened()
{
  if (EventManager)
  {
    // exit previous event
  }

  EventManager = TurnControl->GetManager();
  // enter new one, put actors, etc
}

void AInteractController::StartInteract()
{
  AActor* ChosenObject = GeneralRayCast();
  if (!ChosenObject)
  {
    return;
  }

  AInteractiveObject* InteractiveObject = Cast<AInteractiveObject>(ChosenObject);

  if (!InteractiveObject)
  {
    return;
  }

  //++++ make two functions: one for rmb, other for lmb, 
  //     so that player can pick just to look info (without target choosing)
  //     also make basic function to ray cast for InteractiveObject
  InteractiveObject->Pick();
}

void AInteractController::StopInteract()
{
  if (CurrentCamera)
  {
    CurrentCamera->PlayerReleasedClick();
  }
}

void AInteractController::BeginPlay()
{
  Super::BeginPlay();

  PlacableCharacters = Cast<UChoicesInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->ChosenCharacters;
}

void AInteractController::SetupInputComponent()
{
  Super::SetupInputComponent();

  InputComponent->BindAction("Interact", IE_Pressed, this, &AInteractController::StartInteract);

  InputComponent->BindAction("Interact", IE_Released, this, &AInteractController::StopInteract);
}
