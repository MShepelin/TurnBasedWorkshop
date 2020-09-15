// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedController.h"
#include "Workshop/ACtorClasses/Managers/RegistrationManager.h"


ATurnBasedController::ATurnBasedController()
{

}

void ATurnBasedController::TryToInteract()
{
  // ray trace etc
  // check if it's interactive object
  // send info to hud

  return;
}

void ATurnBasedController::BeginPlay()
{
  Super::BeginPlay();

  bShowMouseCursor = true;
}

void ATurnBasedController::ConnectToEvent(ARegistrationManager* NewManager)
{
  return;

  // init EventManager if it's needed
  // get save data
  // load characters on screen
  // start turnbased event
}

void ATurnBasedController::SetupInputComponent()
{
  Super::SetupInputComponent();

  InputComponent->BindAction("Interact", IE_Pressed, this, &ATurnBasedController::TryToInteract);
}

void ATurnBasedController::SetPawn(APawn * InPawn)
{
  Super::SetPawn(InPawn);

  CurrentCamera = Cast<ASpryCamera>(InPawn);

  if (!CurrentCamera)
  {
    UE_LOG(LogTemp, Error, TEXT("Pawn must be inherited from SpryCamera"));
  }

  /*
  if (InPawn->GetClass()->ImplementsInterface(UTurnBasedCamera::StaticClass()))
  {
    ITurnBasedCamera* CameraInterface = Cast<ITurnBasedCamera>(InPawn);
    CurrentCamera.SetInterface(CameraInterface);
    CurrentCamera.SetObject(InPawn);
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("Pawn must implement TurnBasedCamera interface"));
  }*/
}

ASpryCamera* ATurnBasedController::GetCurrentCamera() const
{
  return CurrentCamera;
}
