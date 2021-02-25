// Copyright © Dmitriy Shepelin 2021. MIT License.

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

  CTsSystem->InitialiseCTs(GameMode->GetCTIDs());
}

TArray<AInteractiveObject*> ARegistrationManager::FindObjectsByCTs(const TArray<int32>& CTsArray, int32 EnoughNumberOfCTs)
{
  return CTsSystem->FindByCTs(CTsArray, EnoughNumberOfCTs);
}

TArray<AInteractiveObject*> ARegistrationManager::FindObjectsByCTsWithMask(const TArray<int32>& CTsArray, int32 EnoughNumberOfCTs, int32 TargetTypeMask)
{
  TArray<AInteractiveObject*> FoundObjects = CTsSystem->FindByCTs(CTsArray, EnoughNumberOfCTs);

  for (size_t ObjectIndex = 0; ObjectIndex < FoundObjects.Num();)
  {
    AInteractiveObject* FoundObject = FoundObjects[ObjectIndex];
    int32 FoundObjectType = FoundObject->GetInteractiveType();

    // Target must have type included in NecessaryMask's possible types
    if (FoundObjectType == (FoundObjectType & TargetTypeMask) && CentralObject != FoundObject)
    {
      ++ObjectIndex;
      continue;
    }

    FoundObjects.RemoveAtSwap(ObjectIndex, 1);
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

void ARegistrationManager::DisconnectObject(AInteractiveObject* Object)
{
  CTsSystem->RemoveObject(Object);
  if (CentralObject == Object)
  {
    Object->UnpickedAsCentral();
  }
}

void ARegistrationManager::ConnectObject(AInteractiveObject* Object)
{
  ARegistrationManager* ObjectsManager = Object->GetManager();

  // Remove previuos Manager if it was chosen for an object
  if (ObjectsManager)
  {
    ObjectsManager->DisconnectObject(Object);
  }

  Object->SetManager(this);

  CTsSystem->AddObject(Object);

  AInteractiveCharacter* ObjectAsCharacter = Cast<AInteractiveCharacter>(Object);
  if (ObjectAsCharacter && (ObjectAsCharacter->GetInteractiveType()))
  {
    for (AInteractiveAbility* CharactersAbility : *ObjectAsCharacter->GetAbilties())
    {
      ConnectObject(CharactersAbility);
    }
  }
}

void ARegistrationManager::PutToSleepManagedObjects(ARegistrationManager* Manager)
{
  while (AwakenObjects.Num())
  {
    AwakenObjects.Pop()->Sleep();
  }
}

void ARegistrationManager::AwakeByCenterObject(TArray<AInteractiveObject*>& Objects)
{
  for (AInteractiveObject* Object : Objects)
  {
    AwakenObjects.Add(Object);
    Object->AwakeBy(CentralObject);
  }
}
