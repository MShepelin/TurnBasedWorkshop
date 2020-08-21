// Fill out your copyright notice in the Description page of Project Settings.

#include "RegistrationManager.h"


// Sets default values
ARegistrationManager::ARegistrationManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARegistrationManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARegistrationManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARegistrationManager::PostInitProperties()
{
  Super::PostInitProperties();
  for (TPair<int32, FString>& Tag : TagsWithNames)
  {
    TagsSystem.AddTag(Tag.Key);
  }
}

void ARegistrationManager::ConnectObjectToTagsSystem(AInteractiveObject* ObjectToAdd)
{
  TagsSystem.AddObject(ObjectToAdd);
  ObjectToAdd->ConnectToManager(this);
}

void ARegistrationManager::DisconnectObjectFromTagsSystem(AInteractiveObject* ObjectToRemove)
{
  TagsSystem.RemoveObject(ObjectToRemove);
}

TArray<AInteractiveObject*> ARegistrationManager::FindObjectsByTags(const TArray<int32> TagsArray, int32 EnoughNumberOfTags) const
{
  TArray<AInteractiveObject*> FoundObjects = TagsSystem.FindByTags(TagsArray, EnoughNumberOfTags);

#if WITH_EDITOR
  // Show what objects are found
  for (AInteractiveObject* FoundObject : FoundObjects)
  {
    DrawDebugLine(GetWorld(), GetActorLocation(), FoundObject->GetActorLocation(),
      DebugColor, false);
  }
#endif

  return FoundObjects;
}