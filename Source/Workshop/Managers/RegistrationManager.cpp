// Fill out your copyright notice in the Description page of Project Settings.

#include "RegistrationManager.h"


ARegistrationManager::ARegistrationManager()
{
	PrimaryActorTick.bCanEverTick = true;

  ManagerIcon = CreateDefaultSubobject<UBillboardComponent>(TEXT("ManagerIcon"));
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
}


void ARegistrationManager::ConnectObjectToManager(AInteractiveObject* ObjectToAdd)
{
  ARegistrationManager* CurrentManagerOfObject = ObjectToAdd->GetManager();

  if (CurrentManagerOfObject)
  {
    CurrentManagerOfObject->DisconnectObjectFromManager(ObjectToAdd);
  }

  // Add to CTs system
  CTsSystem.AddObject(ObjectToAdd);
  ObjectToAdd->SetManager(this);
}


void ARegistrationManager::DisconnectObjectFromManager(AInteractiveObject* ObjectToRemove)
{
  CTsSystem.RemoveObject(ObjectToRemove);
  ObjectToRemove->SetManager(nullptr);
}


TArray<AInteractiveObject*> ARegistrationManager::FindObjectsByCTs(const TArray<int32> TagsArray, int32 EnoughNumberOfTags) const
{
  TArray<AInteractiveObject*> FoundObjects = CTsSystem.FindByCTs(TagsArray, EnoughNumberOfTags);

#if WITH_EDITOR
  // Show what objects were found
  for (AInteractiveObject* FoundObject : FoundObjects)
  {
    DrawDebugLine(GetWorld(), GetActorLocation(), FoundObject->GetActorLocation(),
      DebugColor, false, DebugTime);
  }
#endif

  return FoundObjects;
}


FString ARegistrationManager::GetCTName(int32 TagIdentifier) const
{
  if (!CTsToNameMap.Find(TagIdentifier))
  {
    UE_LOG(LogTemp, Error, TEXT("Wrong tag identifier!"));
    return "";
  }
  
  return CTsToNameMap[TagIdentifier];
}


FString ARegistrationManager::GetStatNameByID(int32 StatIdentifier) const
{
  if (!StatIDToNameMap.Find(StatIdentifier))
  {
    UE_LOG(LogTemp, Error, TEXT("Wrong tag identifier!"));
    return "";
  }

  return StatIDToNameMap[StatIdentifier];
}
