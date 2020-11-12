// ...

#include "FightController.h"
#include "Workshop/ActorClasses/CharactersCollection.h"
#include "Workshop/GameControl/GameModes/FightGameMode.h"

AFightController::AFightController()
{
  TurnControl = CreateDefaultSubobject<UTurnBasedComponent>(TEXT("TurnControl"));
  AddOwnedComponent(TurnControl);
  TurnControl->ConnectDelegate.BindUObject(this, &AFightController::ConnectionHappened);
  TurnControl->TurnIsTakenUnderControl.BindUObject(this, &AFightController::TurnControllGained);
}

void AFightController::TurnControllGained()
{
  while (UsedManager->GetPhase() != ETurnPhase::End)
  {
    UsedManager->NextPhase();
  }

  UsedManager->NextPhase();
}

void AFightController::ConnectionHappened()
{
  if (!GetPawn())
  {
    UE_LOG(LogTemp, Error, TEXT("Controller must possess a CharactersCollection to spawn characters"));
    return;
  }

  ACharactersCollection* PossessedPawn = Cast<ACharactersCollection>(GetPawn());
  if (!PossessedPawn)
  {
    UE_LOG(LogTemp, Error, TEXT("Possessed pawn must inherit from CharactersCollection"));
    return;
  }

  // Find new manager. //++++ change this part for more general case
  AFightGameMode* GameMode = Cast<AFightGameMode>(GetWorld()->GetAuthGameMode());
  check(GameMode);
  UsedManager = GameMode->FightManager;
  check(UsedManager != nullptr);

  CharactersSpawnTransforms.Sort(
    [](const TPair<int32, FTransform>& Left, const TPair<int32, FTransform>& Right) { return Left.Key < Right.Key; }); //++++ move somewhere else

  FActorSpawnParameters SpawnParams = FActorSpawnParameters();
  SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

  for (TSubclassOf<AInteractiveCharacter> SpawnClass : PossessedPawn->CharacterClasses)
  {
    AInteractiveCharacter* NewCharacter = GetWorld()->SpawnActor<AInteractiveCharacter>(SpawnClass, SpawnParams);
    NewCharacter->RefreshInteractive();
    SpawnedCharacters.Add(NewCharacter);
  }

  int32 MaxLocations = CharactersSpawnTransforms.Num();
  check(MaxLocations >= SpawnedCharacters.Num());
  int32 LocationCounter = (MaxLocations - SpawnedCharacters.Num()) / 2;

  FTransform CameraSpawnTransform;
  for (AInteractiveCharacter* PlacableCharacter : SpawnedCharacters)
  {
    CameraSpawnTransform = CharactersSpawnTransforms[LocationCounter].Value;
    PlacableCharacter->SetActorTransform(CameraSpawnTransform);
    LocationCounter++;

    UsedManager->ConnectObject(PlacableCharacter);
  }
}

void AFightController::PostInitializeComponents()
{
  Super::PostInitializeComponents();

  if (!GetWorld())
  {
    return;
  }

  AFightGameMode* GameMode = Cast<AFightGameMode>(GetWorld()->GetAuthGameMode());
  if (!GameMode)
  {
    // throw smth
    return;
  }

  GameMode->FightControllers.Add(
    TPairInitializer<int32, AFightController*>(ControllerID, this));
}
