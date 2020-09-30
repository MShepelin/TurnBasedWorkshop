// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractController.h"
#include "Workshop/ActorClasses/Managers/TurnBasedManager.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveObject.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveCharacter.h"
#include "Workshop/ActorClasses/CameraWork/SpryCamera.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/UObjectBaseUtility.h"
#include "Async/AsyncWork.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealEd.h"
#include "../ChoicesInstance.h"

AInteractController::AInteractController()
{
  TurnControl = CreateDefaultSubobject<UTurnBasedComponent>(TEXT("TurnControl"));
  AddOwnedComponent(TurnControl);
  TurnControl->ConnectDelegate.BindUObject(this, &AInteractController::ConnectionHappened);

  bSwapModeIsActive = false;
  FirstToSwap[0] = FirstToSwap[1] = nullptr;
}

void AInteractController::ConnectionHappened()
{
  if (EventManager)
  {
    EventManager->RemoveController(this);
  }

  EventManager = TurnControl->GetManager();

  int32 MaxLocations = GetCurrentCamera()->SpawnLocations->GetInstanceCount();
  check(MaxLocations >= PlacableCharacters.Num());
  FTransform CameraSpawnTransoform;
  int32 LocationCounter = MaxLocations / 2 - PlacableCharacters.Num() / 2;
  for (AInteractiveCharacter* PlacableCharacter : PlacableCharacters)
  {
    GetCurrentCamera()->SpawnLocations->GetInstanceTransform(LocationCounter, CameraSpawnTransoform, true);
    PlacableCharacter->SetActorTransform(CameraSpawnTransoform);
    LocationCounter++;

    EventManager->ConnectObject(PlacableCharacter);
  }
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

  FActorSpawnParameters SpawnParams = FActorSpawnParameters();

  SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

  for (TTuple<FCharacterCore, FInteractiveCore> CharacterData : Cast<UChoicesInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->ChosenCharacters)
  {
    check(SpawnCharacterClass != nullptr);
    AInteractiveCharacter* NewCharacter = GetWorld()->SpawnActor<AInteractiveCharacter>(SpawnCharacterClass, SpawnParams);
    NewCharacter->CharacterDataCore = CharacterData.Get<0>();
    NewCharacter->InteractiveDataCore = CharacterData.Get<1>();
    NewCharacter->SetActorLocation(GetCurrentCamera()->GetHiddenLocation());
    NewCharacter->RefreshInteractive();
    PlacableCharacters.Add(NewCharacter);
  }
}

void AInteractController::SetupInputComponent()
{
  Super::SetupInputComponent();

  InputComponent->BindAction("Interact", IE_Pressed, this, &AInteractController::StartInteract);

  InputComponent->BindAction("Interact", IE_Released, this, &AInteractController::StopInteract);
}

void AInteractController::SetSwapMode(bool bIsActive)
{
  if (bSwapModeIsActive == bIsActive)
  {
    return;
  }

  EventManager->GetCentralObject()->Pick();

  bSwapModeIsActive = bIsActive;
}
