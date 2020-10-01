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

  FActorSpawnParameters SpawnParams = FActorSpawnParameters();
  SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

  for (TTuple<FCharacterCore, FInteractiveCore, TSubclassOf<AInteractiveCharacter>> CharacterData : Cast<UChoicesInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->ChosenCharacters)
  {
    AInteractiveCharacter* NewCharacter = GetWorld()->SpawnActor<AInteractiveCharacter>(CharacterData.Get<2>(), SpawnParams);
    NewCharacter->CharacterDataCore = CharacterData.Get<0>();
    NewCharacter->InteractiveDataCore = CharacterData.Get<1>();
    NewCharacter->SetActorLocation(GetCurrentCamera()->GetHiddenLocation());
    NewCharacter->RefreshInteractive();
    PlacableCharacters.Add(NewCharacter);
  }

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

  if (bSwapModeIsActive)
  {
    AInteractiveCharacter* InteractiveCharacter = Cast<AInteractiveCharacter>(InteractiveObject);

    if (!InteractiveCharacter)
    {
      return;
    }

    if (FirstToSwap[0] != nullptr)
    {
      FirstToSwap[0] = InteractiveCharacter;
    }
    else
    {
      FirstToSwap[1] = InteractiveCharacter;
     
      PlacableCharacters.Swap(PlacableCharacters.Find(FirstToSwap[0]), PlacableCharacters.Find(FirstToSwap[1]));
      
      //++++ add movement
      FTransform FirstTransform = FirstToSwap[0]->GetActorTransform();
      FirstToSwap[0]->SetActorTransform(FirstToSwap[1]->GetActorTransform());
      FirstToSwap[1]->SetActorTransform(FirstTransform);

      FirstToSwap[0] = FirstToSwap[1] = nullptr;
    }

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
}

void AInteractController::SetupInputComponent()
{
  Super::SetupInputComponent();

  InputComponent->BindAction("Interact", IE_Pressed, this, &AInteractController::StartInteract);
  InputComponent->BindAction("Interact", IE_Released, this, &AInteractController::StopInteract);

  // BindAction with SetSwapMode
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
