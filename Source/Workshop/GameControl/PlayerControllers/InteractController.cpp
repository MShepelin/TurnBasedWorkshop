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
#include "Workshop/GameControl/GameModes/FightGameMode.h"
#include "../ChoicesInstance.h"
#include "Workshop/Types/Nonblueprintable/GameConstants.h"
#include "GenericPlatform/GenericPlatformProcess.h"

AInteractController::AInteractController()
{
  bEnableClickEvents = true;
  bEnableMouseOverEvents = true;

  //TurnControl = CreateDefaultSubobject<UTurnBasedComponent>(TEXT("TurnControl"));
  //AddOwnedComponent(TurnControl);
  //TurnControl->ConnectDelegate.BindUObject(this, &AInteractController::ConnectionHappened);
  //TurnControl->TurnIsTakenUnderControl.BindUObject(this, &AInteractController::TurnControllGained);
  //TurnControl->TurnIsOutOfControl.BindUObject(this, &AInteractController::TurnControllLost);

  bSwapModeIsActive = false;
  FirstToSwap[0] = FirstToSwap[1] = nullptr;
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

    auto& PlacableCharacters = PossessedObserver->GetSpawnedCharactes();
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

  if (!UsedAbilitiesWidget)
  {
    return;
  }

  if (bSwapModeIsActive)
  {
    UsedAbilitiesWidget->SwapText->SetText(UsedAbilitiesWidget->SwapIsActiveText);
  }
  else
  {
    UsedAbilitiesWidget->SwapText->SetText(UsedAbilitiesWidget->SwapIsInactiveText);
  }
}

void AInteractController::ResolveCharactersAbilities()
{
  for (AInteractiveCharacter* PlacableCharacter : PossessedObserver->GetSpawnedCharactes())
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

  UpdatePhaseInfo(); //++++ move to UsedAbilitiesWidget
}

UAbilitiesWidget* AInteractController::GetAbilitiesWidget()
{
  return UsedAbilitiesWidget;
}

void AInteractController::PostInitializeComponents()
{
  Super::PostInitializeComponents();
}

void AInteractController::PlayerWantsToChangePhase()
{
  if (!bTurnIsControlled)
  {
    CantPickCallback();
    return;
  }
  
  // Take control from the player
  bTurnIsControlled = false;

  ATurnBasedManager* Manager;
  if (nullptr == (Manager = Cast<ATurnBasedManager>(UsedManager)))
  {
    return;
  }

  if (Manager->GetPhase() == ETurnPhase::AbilitiesEffect)
  {
    bCanPick = false;

    if (Manager->HasCentralObject())
    {
      Manager->GetCentralObject()->UnpickedAsCentral();
    }

    // Reset thread
    if (ResolveThread)
    {
      ResolveThread->Kill(false);
      ResolveRunnable.reset();
    }

    // Remove abilities from the player's screen
    for (AInteractiveCharacter* ControlledCharacter : PossessedObserver->GetSpawnedCharactes())
    {
      // Pointers to central abilities must stay valid for their characters
      ControlledCharacter->ClearCentralAbility(true);

      // Preparation step is needed to store all effects in a thread-safe place
      ControlledCharacter->PrepareCentralAbilityToResolve();
    }

    ResolveRunnable = std::make_shared<FCharactersResolve>(this);
    ResolveThread = FRunnableThread::Create(ResolveRunnable.get(), TEXT("Characters Resolvement"));

    Manager->NextPhase();
    UpdatePhaseInfo();

    //bTurnIsControlled will be changed by the thread
    return;
  }

  if (Manager->GetPhase() == ETurnPhase::End)
  {
    for (AInteractiveCharacter* PlacableCharacter : PossessedObserver->GetSpawnedCharactes())
    {
      for (AInteractiveAbility* Ability : PlacableCharacter->Abilities)
      {
        Ability->UpdateAfterResolution();
        Ability->DecreaseEffectsDuration();
        Ability->UpdateExhaust();
      }

      PlacableCharacter->ClearCentralAbility();
    }
  }

  Manager->NextPhase();
  UpdatePhaseInfo();

  // Return control to the player
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
    UsedAbilitiesWidget->NextPhaseText->SetText(FText::FromString("End turn"));
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

void AInteractController::OnConnectToManager_Implementation()
{
  PossessedObserver->SpawnCharacters();
  PossessedObserver->ConnectAllSpawnedObjects();
  UsedManager = PossessedObserver->GetManager();
}

void AInteractController::OnDisconnectFromManager_Implementation()
{
  PossessedObserver->RemoveCharacters();
  UsedManager = nullptr;
}

void AInteractController::OnGetTurnControl_Implementation()
{
  bTurnIsControlled = true;
  bCanPick = true;
}

void AInteractController::OnLoseTurnControl_Implementation()
{
  bTurnIsControlled = false;
}

void AInteractController::SetPawn(APawn *InPawn)
{
  Super::SetPawn(InPawn);

  ATurnBasedObserver* PotentialObserver = Cast<ATurnBasedObserver>(InPawn);
  if (!PotentialObserver)
  {
    UE_LOG(LogTemp, Warning, TEXT("AInteractController is designed to controll TurnBasedObserver, but the possessed pawn is not"));
    return;
  }

  PossessedObserver = PotentialObserver;

  UChoicesInstance* Choices = Cast<UChoicesInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
  if (Choices->ChosenCharacterClasses.Num())
  {
    PossessedObserver->SetCharactersToUse(
      Choices->ChosenCharacterClasses, 
      &Choices->ChosenCharacterOptions,
      &Choices->ChosenInteractiveOptions
    );
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
