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
#include "Workshop/Types/Nonblueprintable/GameConstants.h"

AInteractController::AInteractController()
{
  TurnControl = CreateDefaultSubobject<UTurnBasedComponent>(TEXT("TurnControl"));
  AddOwnedComponent(TurnControl);
  TurnControl->ConnectDelegate.BindUObject(this, &AInteractController::ConnectionHappened);
  TurnControl->TurnIsTakenUnderControl.BindUObject(this, &AInteractController::TurnControllGained);
  TurnControl->TurnIsOutOfControl.BindUObject(this, &AInteractController::TurnControllLost);

  bSwapModeIsActive = false;
  FirstToSwap[0] = FirstToSwap[1] = nullptr;
}

void AInteractController::ConnectionHappened()
{
  // Disconnect from previous manager.
  ATurnBasedManager* EventManager = Cast<ATurnBasedManager>(UsedManager);
  if (EventManager)
  {
    EventManager->RemoveController(this);
  }

  // Find new manager. //++++ change this part for more general case
  AFightGameMode* GameMode = Cast<AFightGameMode>(GetWorld()->GetAuthGameMode());
  check(GameMode);
  EventManager = GameMode->FightManager;

  // Spawn characters.
  FActorSpawnParameters SpawnParams = FActorSpawnParameters();
  SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
  for (TTuple<FCharacterCore, FInteractiveCore, TSubclassOf<AInteractiveCharacter>> CharacterData : Cast<UChoicesInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->ChosenCharacters)
  {
    AInteractiveCharacter* NewCharacter = GetWorld()->SpawnActor<AInteractiveCharacter>(CharacterData.Get<2>(), SpawnParams);
    NewCharacter->CharacterDataCore = CharacterData.Get<0>();
    NewCharacter->InteractiveDataCore = CharacterData.Get<1>();
    NewCharacter->RefreshInteractive();
    PlacableCharacters.Add(NewCharacter);
  }

  // Give them to appropriate transforms.
  int32 MaxLocations = CharactersSpawnTransforms.Num();
  check(MaxLocations >= PlacableCharacters.Num());
  FTransform CameraSpawnTransoform;
  int32 LocationCounter = (MaxLocations - PlacableCharacters.Num()) / 2;
  for (AInteractiveCharacter* PlacableCharacter : PlacableCharacters)
  {
    CameraSpawnTransoform = CharactersSpawnTransforms[LocationCounter].Value;
    PlacableCharacter->SetActorTransform(CameraSpawnTransoform);
    LocationCounter++;

    EventManager->ConnectObject(PlacableCharacter);
  }

  // Set new manager.
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
  // It's guarantied that before the call of LinkWithAbilitiesWidget
  // UsedManager is inherited from TurnBasedManager

  if (UsedAbilitiesWidget)
  {
    UsedAbilitiesWidget->NextPhaseButton->OnPressed.Clear();
    UsedAbilitiesWidget->TurnSwapButton->OnPressed.Clear();
  }

  UsedAbilitiesWidget = AbilitiesWidget;
  AbilitiesWidget->NextPhaseButton->OnPressed.AddDynamic(this, &AInteractController::PlayerWantsToChangePhase); //Cast<ATurnBasedManager>(UsedManager), &ATurnBasedManager::NextPhase);
  AbilitiesWidget->NextPhaseButton->OnPressed.AddDynamic(UsedAbilitiesWidget, &UAbilitiesWidget::PhaseChange);
  AbilitiesWidget->TurnSwapButton->OnPressed.AddDynamic(this, &AInteractController::TurnSwapMode);

  AbilitiesWidget->ConsideredTurnPhase = &(Cast<ATurnBasedManager>(UsedManager)->PhaseNameBuffer);

  if (bSwapModeIsActive)
  {
    AbilitiesWidget->SwapText->SetText(UsedAbilitiesWidget->SwapIsActiveText);
  }
  else
  {
    AbilitiesWidget->SwapText->SetText(UsedAbilitiesWidget->SwapIsInactiveText);
  }

  UpdatePhaseInfo();
}

UAbilitiesWidget* AInteractController::GetAbilitiesWidget()
{
  return UsedAbilitiesWidget;
}

void AInteractController::AddSpawnTransform(FTransform NewSpawn, int32 Order)
{
  CharactersSpawnTransforms.Add(TPairInitializer<int32, FTransform>(Order, NewSpawn));
}

void AInteractController::PostInitializeComponents()
{
  Super::PostInitializeComponents();

  AFightGameMode* GameMode = Cast<AFightGameMode>(GetWorld()->GetAuthGameMode());
  check(GameMode);

  GameMode->ObjectsReady.AddUObject(this, &AInteractController::PrepareCharacters);
}

void AInteractController::PrepareCharacters()
{
  // Sort transforms by orderID
  CharactersSpawnTransforms.Sort([](const TPair<int32, FTransform>& Left, const TPair<int32, FTransform>& Right) { return Left.Key < Right.Key; });
}

void AInteractController::TurnControllGained()
{
  bLevelIsControlled = true;
}

void AInteractController::TurnControllLost()
{
  bLevelIsControlled = false;
}

void AInteractController::PlayerWantsToChangePhase()
{
  if (!bLevelIsControlled)
  {
    return;
  }

  ATurnBasedManager* Manager;
  if ((Manager = Cast<ATurnBasedManager>(UsedManager)) == nullptr)
  {
    return;
  }

  if (Manager->GetPhase() == )
  {

  }

  Manager->NextPhase();

  UpdatePhaseInfo();
}

void AInteractController::UpdatePhaseInfo()
{
  ATurnBasedManager* Manager;

  if (UsedAbilitiesWidget == nullptr || (Manager = Cast<ATurnBasedManager>(UsedManager)) == nullptr)
  {
    return;
  }

  ETurnPhase Phase = Manager->GetPhase();
  UsedAbilitiesWidget->PhaseText->SetText(FText::FromString(UEnum::GetValueAsString(Phase).RightChop(LengthOfPhaseTypeName)));

  if (Phase == ETurnPhase::End)
  {
    UsedAbilitiesWidget->NextPhaseText->SetText(FText::FromString("Next turn"));
    return;
  }
  else
  {
    Phase = static_cast<ETurnPhase>(static_cast<uint8>(Phase) + 1);
  }

  UsedAbilitiesWidget->NextPhaseText->SetText(FText::FromString(UEnum::GetValueAsString(Phase).RightChop(LengthOfPhaseTypeName)));
}
