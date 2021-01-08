// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractController.h"
#include "Workshop/ActorClasses/Managers/TurnBasedManager.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveObject.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveCharacter.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveAbility.h"
#include "Workshop/ActorClasses/CameraWork/SpryCamera.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/UObjectBaseUtility.h"
#include "Async/AsyncWork.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealEd.h"
#include "Workshop/GameControl/GameModes/FightGameMode.h"
#include "../ChoicesInstance.h"
#include "Workshop/Types/Nonblueprintable/GameConstants.h"
#include "GenericPlatform/GenericPlatformProcess.h"

AInteractController::AInteractController()
{
  bEnableClickEvents = true;
  bEnableMouseOverEvents = true;

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

  // Give characters appropriate transforms.
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
  if (CurrentCamera)
    CurrentCamera->PlayerPressedClick();

  AActor* ChosenObject = GeneralRayCast();
  if (!ChosenObject)
    return;

  AInteractiveObject* InteractiveObject = Cast<AInteractiveObject>(ChosenObject);

  if (!InteractiveObject)
    return;

  if (!bCanPick)
  {
    CantPickCallback();
    return;
  }

  if (!bSwapModeIsActive)
  {
    InteractiveObject->Pick();
    return;
  }

  // If the object is InteractiveCharacter, try to apply swap
  AInteractiveCharacter* InteractiveCharacter = Cast<AInteractiveCharacter>(InteractiveObject);

  if (!InteractiveCharacter || InteractiveCharacter->CharacterDataCore.bIsExhausted)
    return;

  if (FirstToSwap[0])
  {
    FirstToSwap[1] = InteractiveCharacter;

    PlacableCharacters.Swap(PlacableCharacters.Find(FirstToSwap[0]), PlacableCharacters.Find(FirstToSwap[1]));

    //++++ add movement
    FTransform FirstTransform = FirstToSwap[0]->GetActorTransform();
    FirstToSwap[0]->SetActorTransform(FirstToSwap[1]->GetActorTransform());
    FirstToSwap[1]->SetActorTransform(FirstTransform);

    FirstToSwap[0] = FirstToSwap[1] = nullptr;
  }
  else
  {
    FirstToSwap[0] = InteractiveCharacter;
  }

  //++++ you can store one pointer in FirstToSwap except of 2

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
  if (!bCanPick)
  {
    CantPickCallback();
    return;
  }

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
  AbilitiesWidget->NextPhaseButton->OnClicked.AddDynamic(this, &AInteractController::PlayerWantsToChangePhase);
  AbilitiesWidget->TurnSwapButton->OnClicked.AddDynamic(this, &AInteractController::TurnSwapMode);

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

  if (GameMode)
  {
    GameMode->ObjectsReady.AddUObject(this, &AInteractController::PrepareCharacters);
  }
}

void AInteractController::PrepareCharacters()
{
  // Sort transforms by orderID
  CharactersSpawnTransforms.Sort([](const TPair<int32, FTransform>& Left, const TPair<int32, FTransform>& Right) { return Left.Key < Right.Key; });
}

void AInteractController::TurnControllGained()
{
  bTurnIsControlled = true;
  bCanPick = true;
}

void AInteractController::TurnControllLost()
{
  bTurnIsControlled = false;
}

void AInteractController::PlayerWantsToChangePhase()
{
  if (!bTurnIsControlled)
  {
    CantPickCallback();
    return;
  }
  
  bTurnIsControlled = false;

  ATurnBasedManager* Manager;
  if ((Manager = Cast<ATurnBasedManager>(UsedManager)) == nullptr)
  {
    // Error
    return;
  }

  if (Manager->GetPhase() == ETurnPhase::AbilitiesEffect)
  {
    bCanPick = false;

    if (Manager->HasCentralObject())
    {
      Manager->GetCentralObject()->UnpickedAsCentral();
    }

    if (ResolveThread)
    {
      // PlayerWantsToChangePhase can't be called until ResolveThread changes bTurnIsControlled. 
      ResolveThread->Kill(false);
      ResolveRunnable.reset();
    }

    ResolveRunnable = std::make_shared<FCharactersResolve>(this);
    ResolveThread = FRunnableThread::Create(ResolveRunnable.get(), TEXT("Characters Resolvement"));

    Manager->NextPhase();
    UpdatePhaseInfo();

    //bTurnIsControlled changed by the thread
    return;
  }

  if (Manager->GetPhase() == ETurnPhase::End)
  {
    for (AInteractiveCharacter* PlacableCharacter : PlacableCharacters)
    {
      for (AInteractiveAbility* Ability : PlacableCharacter->Abilities)
      {
        Ability->UpdateCharacterStatus();
      }
    }
  }

  Manager->NextPhase();
  UpdatePhaseInfo();

  bTurnIsControlled = true;
}

void AInteractController::UpdatePhaseInfo() //???? move to AbilitiesWidget
{
  ATurnBasedManager* Manager;

  if (UsedAbilitiesWidget == nullptr || (Manager = Cast<ATurnBasedManager>(UsedManager)) == nullptr)
  {
    return;
  }

  ETurnPhase Phase = Manager->GetPhase();
  UsedAbilitiesWidget->PhaseText->SetText(FText::FromString(UEnum::GetValueAsString(Phase).RightChop(PHASE_ENUM_NAME_LENGTH)));

  if (Phase == ETurnPhase::End)
  {
    UsedAbilitiesWidget->NextPhaseText->SetText(FText::FromString("Next turn"));
    return;
  }
  else
  {
    Phase = static_cast<ETurnPhase>(static_cast<uint8>(Phase) + 1);
  }

  UsedAbilitiesWidget->NextPhaseText->SetText(FText::FromString(UEnum::GetValueAsString(Phase).RightChop(PHASE_ENUM_NAME_LENGTH)));
}

void AInteractController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);

  if (ResolveRunnable && ResolveThread)
  {
    ResolveThread->Suspend(true);
    ResolveThread->Kill(false); //???? change somehow to prevent problems with threads
    ResolveRunnable.reset();
  }
}

void AInteractController::CantPickCallback()
{
  if (UsedAbilitiesWidget)
  {
    UsedAbilitiesWidget->FailToInteract();
    GetWorldTimerManager().SetTimer(Handler, UsedAbilitiesWidget, &UAbilitiesWidget::ShowBasicText, DEFAULT_DELAY, true);
  }
}

FCharactersResolve::FCharactersResolve(AInteractController* Controller) : UsedController(Controller)
{

}

uint32 FCharactersResolve::Run()
{
  // UE_LOG(LogTemp, Warning, TEXT("Thread is running"));
  UsedController->ResolveCharactersAbilities();
  UsedController->bTurnIsControlled = true;
  return 0;
}

void FCharactersResolve::Exit()
{
  // UE_LOG(LogTemp, Warning, TEXT("Thread exits"));
}
