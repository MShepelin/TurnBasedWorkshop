// Fill out your copyright notice in the Description page of Project Settings.

#include "RegistrationManager.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveObject.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveCharacter.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveAbility.h"
#include "Workshop/WorkshopGameModeBase.h"

ARegistrationManager::ARegistrationManager()
{
	PrimaryActorTick.bCanEverTick = true;

  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MainScene"));;
  ManagerIcon = CreateDefaultSubobject<UBillboardComponent>(TEXT("ManagerIcon"));
  ManagerIcon->SetupAttachment(RootComponent);

  //++++ manage memory in other way, TWeakPtr<>
  CTsSystem = std::make_unique<CTsSearch<int32, AInteractiveObject>>();
}

void ARegistrationManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARegistrationManager::BeginPlay()
{
  Super::BeginPlay();

  UWorld* World = GetWorld();
  if (!World)
  {
    return;
  }

  AWorkshopGameModeBase* GameMode = Cast<AWorkshopGameModeBase>(World->GetAuthGameMode());
  if (!GameMode)
  {
    UE_LOG(LogTemp, Error, TEXT("Manager tried to use GameMode but it isn't inherited from WorkshopGameModeBase"));
    return;
  }

  //++++ change to initialiseCTs
  for (int32 CT : GameMode->GetCTIDs())
  {
    CTsSystem->AddCT(CT);
  }
}

// Find objects with respect to chosen CentralObject.
TArray<AInteractiveObject*> ARegistrationManager::FindObjectsByCTs(const TArray<int32>& CTsArray, int32 EnoughNumberOfCTs)
{
  return CTsSystem->FindByCTs(CTsArray, EnoughNumberOfCTs);
}

TArray<AInteractiveObject*> ARegistrationManager::FindObjectsByCTsWithMask(const TArray<int32>& CTsArray, int32 EnoughNumberOfCTs, int32 TargetTypeMask)
{
  TArray<AInteractiveObject*> FoundObjects = CTsSystem->FindByCTs(CTsArray, EnoughNumberOfCTs);

  int32 AtLeastOneMask = TargetTypeMask & INTERACTIVE_TYPE_SEPARATOR_MASK;
  int32 NecessaryMask = TargetTypeMask & ~INTERACTIVE_TYPE_SEPARATOR_MASK;

  for (size_t ObjectIndex = 0; ObjectIndex < FoundObjects.Num(); ObjectIndex++)
  {
    AInteractiveObject* FoundObject = FoundObjects[ObjectIndex];
    int32 AtLeastOneMaskTarget = (FoundObject->GetInteractiveType() & TargetTypeMask) & INTERACTIVE_TYPE_SEPARATOR_MASK;
    int32 NecessaryMaskTarget = (FoundObject->GetInteractiveType() & TargetTypeMask) & ~INTERACTIVE_TYPE_SEPARATOR_MASK;

    // Target must have type included in NecessaryMask's possible types
    if ((AtLeastOneMaskTarget & AtLeastOneMask) && ((NecessaryMaskTarget & NecessaryMask) == NecessaryMaskTarget) && CentralObject != FoundObject)
    {
      continue;
    }

    FoundObjects.Swap(ObjectIndex, FoundObjects.Num() - 1);
    FoundObjects.Pop();
  }

  return FoundObjects;
}

TArray<AInteractiveObject*> ARegistrationManager::GetAllConnectedObjects() const
{
  return CTsSystem->GetAllObjects();
}

void ARegistrationManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);

  CTsSystem.reset();
}

bool ARegistrationManager::HasCentralObject() const
{
  return CentralObject != nullptr;
}

AInteractiveObject* ARegistrationManager::GetCentralObject()
{
  return CentralObject;
}

void ARegistrationManager::ConnectObject(AInteractiveObject* Object)
{
  ARegistrationManager*& ObjectsManager = Object->MainManager;

  // Remove previuos Manager if it was chosen for an object
  //???? check if object is already connected
  if (ObjectsManager)
  {
    ObjectsManager->CTsSystem->RemoveObject(Object);

    if (ObjectsManager->CentralObject == Object)
    {
      Object->UnpickedAsCentral();
    }
  }

  ObjectsManager = this;
  CTsSystem->AddObject(Object);

  AInteractiveCharacter* ObjectAsCharacter = Cast<AInteractiveCharacter>(Object);
  if (ObjectAsCharacter)
  {
    for (AInteractiveAbility* CharactersAbility : ObjectAsCharacter->Abilities)
    {
      ConnectObject(CharactersAbility);
    }
  }
}

void ARegistrationManager::PutToSleepManagedObjects(ARegistrationManager* Manager)
{
  while (AwakenObjects.Num())
  {
    AwakenObjects.Pop()->InteractivityIcon->Hide();
  }
}

void ARegistrationManager::AwakeByCenterObject(TArray<AInteractiveObject*>& Objects)
{
  for (AInteractiveObject* Object : Objects)
  {
    AwakenObjects.Add(Object);

    // Using friend status not to add new functions
    Object->InteractivityIcon->Show();

    if (CentralObject->InfluencesOn.Find(Object))
    {
      Object->InteractivityIcon->SetIconState(EIconState::ChosenTarget);
    }
    else
    {
      Object->InteractivityIcon->SetIconState(EIconState::AvailableTarget);
    }
  }
}
