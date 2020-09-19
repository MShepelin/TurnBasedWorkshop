// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractController.h"
#include "Workshop/ActorClasses/Managers/RegistrationManager.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveObject.h"
#include "Workshop/ActorClasses/CameraWork/SpryCamera.h"


AInteractController::AInteractController()
{

}

void AInteractController::StartInteract()
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

  bShowMouseCursor = true;
}

/* REMAKE
void AInteractController::ConnectToEvent(ARegistrationManager* NewManager)
{
  return;

  // get save data
  // load characters on screen
  // get TurnWaiter
}
*/

void AInteractController::SetupInputComponent()
{
  Super::SetupInputComponent();

  InputComponent->BindAction("Interact", IE_Pressed, this, &AInteractController::StartInteract);

  InputComponent->BindAction("Interact", IE_Released, this, &AInteractController::StopInteract);
}

void AInteractController::SetPawn(APawn * InPawn)
{
  Super::SetPawn(InPawn);

  CurrentCamera = Cast<ASpryCamera>(InPawn);

  if (!CurrentCamera)
  {
    UE_LOG(LogTemp, Error, TEXT("Pawn must be inherited from SpryCamera"));
  }
}

ASpryCamera* AInteractController::GetCurrentCamera() const
{
  return CurrentCamera;
}
