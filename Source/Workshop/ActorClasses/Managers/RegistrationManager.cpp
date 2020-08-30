// Fill out your copyright notice in the Description page of Project Settings.

#include "RegistrationManager.h"


ARegistrationManager::ARegistrationManager()
{
	PrimaryActorTick.bCanEverTick = true;

  ManagerIcon = CreateDefaultSubobject<UBillboardComponent>(TEXT("ManagerIcon"));
  RootComponent = ManagerIcon;
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
    CTsSystem.AddCT(CT.Key);
  }

  for (int32 CT : NecessaryCTs)
  {
    CTsSystem.AddCT(CT);
  }

  NecessaryCTs.Empty();
  

  //Check if all must-have properties are present
  for (int32 NecessaryStat : {ObjectNameStatID})
  {
    if (!StatIDToNameMap.Find(NecessaryStat))
    {
      UE_LOG(LogTemp, Error, TEXT("Incorrect manager description: some necessary stats are missing!"));
    }
  }
}

TArray<AInteractiveObject*> ARegistrationManager::FindObjectsByCTs(const TArray<int32> CTsArray, int32 EnoughNumberOfCTs) const
{
  TArray<AInteractiveObject*> FoundObjects = CTsSystem.FindByCTs(CTsArray, EnoughNumberOfCTs);

  return FoundObjects;
}

TArray<AInteractiveObject*> ARegistrationManager::GetAllConnectedObjects() const
{
  return CTsSystem.GetAllObjects();
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

  CTsSystem.InitialiseCTs({});
}

bool ARegistrationManager::HasCentralObject() const
{
  return CentralObject != nullptr;
}
