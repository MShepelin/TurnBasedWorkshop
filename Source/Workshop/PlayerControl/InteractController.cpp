// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractController.h"
#include "Workshop/ActorClasses/Managers/TurnBasedManager.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveObject.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveCharacter.h"
#include "Workshop/ActorClasses/CameraWork/SpryCamera.h"
#include "Kismet/GameplayStatics.h"
#include "ProgressData.h"


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
  UE_LOG(LogTemp, Warning, TEXT("GOt you!"));

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
  GetWorld()->LineTraceSingleByChannel(
    Hit,
    CameraLocation,
    CameraLocation + CameraPawn->GetCameraDirection() * RayCastLength,
    ECollisionChannel::ECC_Visibility,
    CollisionParams,
    ResponseParams
  );
  /*
  DrawDebugLine(
    GetWorld(),
    CameraLocation,
    CameraLocation + CameraPawn->GetCameraDirection() * RayCastLength,
    FColor(255, 0, 0),
    false, 5, 0,
    12.333
  );*/

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

  PlacableCharacters = Cast<UChoicesInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->ChosenCharacters;
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
