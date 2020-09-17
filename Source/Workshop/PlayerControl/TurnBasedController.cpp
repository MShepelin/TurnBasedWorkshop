// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedController.h"
#include "Workshop/ActorClasses/Managers/RegistrationManager.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveObject.h"
#include "Workshop/ActorClasses/CameraWork/SpryCamera.h"


ATurnBasedController::ATurnBasedController()
{

}

void ATurnBasedController::TryToInteract()
{
  if (CurrentCamera)
  {
    CurrentCamera->PlayerPressedClick();
  }

  ASpryCamera* CameraPawn = Cast<ASpryCamera>(GetPawn());

  if (!(CameraPawn && GetWorld()))
  {
    return;
  }

  FVector CameraLocation = CameraPawn->GetActorLocation();

  FCollisionQueryParams CollisionParams;
  FCollisionResponseParams ResponseParams;

  FHitResult Hit;
  bool bIsHit = GetWorld()->LineTraceSingleByChannel(
    Hit,
    CameraLocation,
    CameraLocation + CameraPawn->GetCameraDirection() * RayCastLength,
    ECollisionChannel::ECC_Visibility,
    CollisionParams,
    ResponseParams
  );

  if (!bIsHit)
  {
    return;
  }

  AInteractiveObject* InteractiveObject = Cast<AInteractiveObject>(Hit.Actor);

  if (!InteractiveObject)
  {
    return;
  }

  InteractiveObject->Pick();
}

void ATurnBasedController::StopInteract()
{
  if (CurrentCamera)
  {
    CurrentCamera->PlayerReleasedClick();
  }
}

void ATurnBasedController::BeginPlay()
{
  Super::BeginPlay();

  bShowMouseCursor = true;
}

/* REMAKE
void ATurnBasedController::ConnectToEvent(ARegistrationManager* NewManager)
{
  return;

  // get save data
  // load characters on screen
  // get TurnWaiter
}
*/

void ATurnBasedController::SetupInputComponent()
{
  Super::SetupInputComponent();

  InputComponent->BindAction("Interact", IE_Pressed, this, &ATurnBasedController::TryToInteract);

  InputComponent->BindAction("Interact", IE_Released, this, &ATurnBasedController::StopInteract);
}

void ATurnBasedController::SetPawn(APawn * InPawn)
{
  Super::SetPawn(InPawn);

  CurrentCamera = Cast<ASpryCamera>(InPawn);

  if (!CurrentCamera)
  {
    UE_LOG(LogTemp, Error, TEXT("Pawn must be inherited from SpryCamera"));
  }
}

ASpryCamera* ATurnBasedController::GetCurrentCamera() const
{
  return CurrentCamera;
}
