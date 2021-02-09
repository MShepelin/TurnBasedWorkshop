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
    CameraLocation + CameraPawn->GetCameraDirection() * RAY_CAST_LENGTH,
    COLLISION_INTERACTIVE,
    CollisionParams,
    ResponseParams
  );

  //DrawDebugLine(GetWorld(), CameraLocation, CameraLocation + CameraPawn->GetCameraDirection() * RAY_CAST_LENGTH, FColor(0, 255, 0), false, 10);

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

  ASpryCamera* PotentialCamera = Cast<ASpryCamera>(InPawn);
  if (PotentialCamera)
  {
    CurrentCamera = PotentialCamera;
  }
  else
  {
    UE_LOG(LogTemp, Warning, TEXT("ACameraController is designed to controll Spry Camera, but the possessed pawn is not"));
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
