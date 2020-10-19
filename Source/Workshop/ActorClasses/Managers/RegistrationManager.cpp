// Fill out your copyright notice in the Description page of Project Settings.

#include "RegistrationManager.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveObject.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveCharacter.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveAbility.h"


ARegistrationManager::ARegistrationManager()
{
	PrimaryActorTick.bCanEverTick = true;

  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MainScene"));;
  ManagerIcon = CreateDefaultSubobject<UBillboardComponent>(TEXT("ManagerIcon"));
  ManagerIcon->SetupAttachment(RootComponent);

  //++++ manage memory in other way, TWeakPtr<>
  CTsSystem = new CTsGraph<int32, AInteractiveObject>();
}

void ARegistrationManager::BeginPlay()
{
	Super::BeginPlay();
}

void ARegistrationManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARegistrationManager::PostInitProperties()
{
  Super::PostInitProperties();
  for (TPair<int32, FString>& CT : CTsToNameMap)
  {
    CTsSystem->AddCT(CT.Key);
  }

#ifdef WITH_EDITOR
  //Check if any of already used stat IDs are met.
  for (TTuple<int32, FString>& NecessaryStat : StatIDToNameMap)
  {
    if (NecessaryStat.Get<0>() >= 0 && NecessaryStat.Get<0>() < CharacterIntegerStats)
    {
      UE_LOG(LogTemp, Error, TEXT("Stat IDs from 0 to %d are allocated and can't be used!"), CharacterIntegerStats - 1);
    }
  }
#endif
}

// Find objects with respect to chosen CentralObject.
TArray<AInteractiveObject*> ARegistrationManager::FindObjectsByCTs(const TArray<int32>& CTsArray, int32 EnoughNumberOfCTs)
{
  return CTsSystem->FindByCTs(CTsArray, EnoughNumberOfCTs);
}

TArray<AInteractiveObject*> ARegistrationManager::FindObjectsByCTsWithMask(const TArray<int32>& CTsArray, int32 EnoughNumberOfCTs, int32 TargetTypeMask)
{
  TArray<AInteractiveObject*> FoundObjects = CTsSystem->FindByCTs(CTsArray, EnoughNumberOfCTs);

  int32 AtLeastOneMask = TargetTypeMask & InteractiveTypeSeparator;
  int32 NecessaryMask = TargetTypeMask & ~InteractiveTypeSeparator;

  for (size_t ObjectIndex = 0; ObjectIndex < FoundObjects.Num(); ObjectIndex++)
  {
    //REWORK, remove InteractiveTypeSeparator
    AInteractiveObject* FoundObject = FoundObjects[ObjectIndex];
    int32 AtLeastOneMaskTarget = (FoundObject->GetInteractiveType() & TargetTypeMask) & InteractiveTypeSeparator;
    int32 NecessaryMaskTarget = (FoundObject->GetInteractiveType() & TargetTypeMask) & ~InteractiveTypeSeparator;

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

FString ARegistrationManager::GetCTName(int32 CTIdentifier) const
{
  if (!CTsToNameMap.Find(CTIdentifier))
  {
    UE_LOG(LogTemp, Error, TEXT("Wrong CT identifier!"));
    return "";
  }
  
  return CTsToNameMap[CTIdentifier];
}

FString ARegistrationManager::GetStatNameByID(int32 StatIdentifier) const
{
  if (!StatIDToNameMap.Find(StatIdentifier))
  {
    UE_LOG(LogTemp, Error, TEXT("Wrong Stat identifier!"));
    return "";
  }

  return StatIDToNameMap[StatIdentifier];
}

void ARegistrationManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);

  delete CTsSystem;
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

    if (CentralObject->InfluencesSet.Find(Object))
    {
      Object->InteractivityIcon->SetIconState(EIconState::ChosenTarget);
    }
    else
    {
      Object->InteractivityIcon->SetIconState(EIconState::AvailableTarget);
    }
  }
}
