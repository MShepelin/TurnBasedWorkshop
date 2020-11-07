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
#include "Workshop/GameControl/GameModes/FightGameMode.h"
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
  ATurnBasedManager* EventManager = Cast<ATurnBasedManager>(UsedManager);
  // Disconnect from previous manager
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
    //GetCurrentCamera()->SpawnLocations->GetInstanceTransform(LocationCounter, CameraSpawnTransoform, true);
    CameraSpawnTransoform = CharactersSpawnTransforms[LocationCounter];
    PlacableCharacter->SetActorTransform(CameraSpawnTransoform);
    LocationCounter++;

    EventManager->ConnectObject(PlacableCharacter);
  }

  UsedManager = EventManager;
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

  if (!bSwapModeIsActive)
  {
    InteractiveObject->Pick();
    return;
  }

  AInteractiveCharacter* InteractiveCharacter = Cast<AInteractiveCharacter>(InteractiveObject);

  if (!InteractiveCharacter)
  {
    return;
  }

  if (FirstToSwap[0] == nullptr)
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

  //++++ make two functions: one for rmb, other for lmb, 
  //     so that player can pick just to look info (without target choosing)
  //     also make basic function to ray cast for InteractiveObject
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
}

void AInteractController::ObjectsReady()
{
  AFightGameMode* GameMode = Cast<AFightGameMode>(GetWorld()->GetAuthGameMode());
  check(GameMode);

  GameMode->RegisterAllSpawnLocations.Broadcast();
  // sort transforms by orderID
}

void AInteractController::TurnSwapMode()
{
  bSwapModeIsActive = !bSwapModeIsActive;

  // Unpick central object if needed
  if (UsedManager->HasCentralObject())
  {
    UsedManager->GetCentralObject()->UnpickedAsCentral();
  }

  if (UsedAbilitiesWidget)
  {
    if (bSwapModeIsActive)
    {
      UsedAbilitiesWidget->SwapText->SetText(UsedAbilitiesWidget->SwapIsActiveText);
    }
    else
    {
      UsedAbilitiesWidget->SwapText->SetText(UsedAbilitiesWidget->SwapIsInactiveText);
    }
  }
}

void AInteractController::ResolveCharactersAbilities()
{
  for (AInteractiveCharacter* PlacableCharacter : PlacableCharacters)
  {
    PlacableCharacter->ResolveCharacterActions();
    UE_LOG(LogTemp, Warning, TEXT("Character resolved"));
  }
}

void AInteractController::LinkWithAbilitiesWidget(UAbilitiesWidget* AbilitiesWidget)
{
  if (UsedAbilitiesWidget)
  {
    UsedAbilitiesWidget->NextPhaseButton->OnPressed.Clear();
    UsedAbilitiesWidget->TurnSwapButton->OnPressed.Clear();
  }

  UsedAbilitiesWidget = AbilitiesWidget;
  AbilitiesWidget->NextPhaseButton->OnPressed.AddDynamic(TurnControl, &UTurnBasedComponent::NextPhase);
  AbilitiesWidget->TurnSwapButton->OnPressed.AddDynamic(this, &AInteractController::TurnSwapMode);

  if (bSwapModeIsActive)
  {
    AbilitiesWidget->SwapText->SetText(UsedAbilitiesWidget->SwapIsActiveText);
  }
  else
  {
    AbilitiesWidget->SwapText->SetText(UsedAbilitiesWidget->SwapIsInactiveText);
  }
}

UAbilitiesWidget* AInteractController::GetAbilitiesWidget()
{
  return UsedAbilitiesWidget;
}

void AInteractController::AddSpawnTransform(FTransform NewSpawn)
{
  CharactersSpawnTransforms.Add(NewSpawn);
}
