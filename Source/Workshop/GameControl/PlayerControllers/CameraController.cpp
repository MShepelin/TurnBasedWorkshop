// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraController.h"
#include "Workshop/ActorClasses/Managers/TurnBasedManager.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveObject.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveCharacter.h"
#include "Workshop/ActorClasses/CameraWork/SpryCamera.h"
#include "Kismet/GameplayStatics.h"


ACameraController::ACameraController()
{

}

AActor* ACameraController::GeneralRayCast()
{
  ASpryCamera* CameraPawn = Cast<ASpryCamera>(GetPawn());

  if (!(CameraPawn && GetWorld()))
  {
    return nullptr;
  }

  FVector CameraLocation = CameraPawn->GetCameraLocation();

  FCollisionQueryParams CollisionParams;
  FCollisionResponseParams ResponseParams;

  FHitResult Hit;
  GetWorld()->LineTraceSingleByChannel(
    Hit,
    CameraLocation,
    CameraLocation + CameraPawn->GetCameraDirection() * RayCastLength,
    COLLISION_INTERACTIVE,
    CollisionParams,
    ResponseParams
  );

  //DrawDebugLine(GetWorld(), CameraLocation, CameraLocation + CameraPawn->GetCameraDirection() * RayCastLength, FColor(0, 255, 0), false, 10);

  return Hit.GetActor();
}

void ACameraController::BeginPlay()
{
  Super::BeginPlay();

  bShowMouseCursor = true;
}

void ACameraController::SetPawn(APawn * InPawn)
{
  Super::SetPawn(InPawn);

  CurrentCamera = Cast<ASpryCamera>(InPawn);

  if (!CurrentCamera)
  {
    UE_LOG(LogTemp, Error, TEXT("Pawn must be inherited from SpryCamera"));
  }
}

ASpryCamera* ACameraController::GetCurrentCamera() const
{
  return CurrentCamera;
}

ARegistrationManager* ACameraController::GetUsedManager()
{
  return UsedManager;
}
