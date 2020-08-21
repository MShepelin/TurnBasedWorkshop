// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveObject.h"


// Sets default values
AInteractiveObject::AInteractiveObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("MainScene"));
  RootComponent = RootScene;

  InteractivityIcon = CreateDefaultSubobject<UIconComponent>(TEXT("TouchIcon"));
  InteractivityIcon->SetupAttachment(RootScene);
}

void AInteractiveObject::OnConstruction(const FTransform & Transform)
{
  Super::OnConstruction(Transform);
  InteractivityIcon->Initialize();
}

void AInteractiveObject::PostInitProperties()
{
  Super::PostInitProperties();
}

// Called when the game starts or when spawned
void AInteractiveObject::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AInteractiveObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractiveObject::AddInfluenceOn(AInteractiveObject * TargetObject)
{
  if (!InfluencesList.Find(TargetObject))
  {
    InfluencesList.Add(TargetObject);
    TargetObject->DependenciesList.Add(this);
  }
  else
  {
    UE_LOG(LogTemp, Warning, TEXT("Addiction failed, objects are already connected"));
  }
}

void AInteractiveObject::RemoveDependenceFrom(AInteractiveObject * TargetObject)
{
  if (DependenciesList.Find(this))
  {
    TargetObject->InfluencesList.Remove(this);
    DependenciesList.Remove(TargetObject);
  }
  else
  {
    UE_LOG(LogTemp, Warning, TEXT("Removal failed, objects are not connected"));
  }
}

void AInteractiveObject::GatherInformation() const
{

}

void AInteractiveObject::ShowInfluences() const
{
  // Show what objects are influenced by this object
  for (AInteractiveObject* DependentObject : InfluencesList)
  {
    DrawDebugLine(GetWorld(), GetActorLocation(), DependentObject->GetActorLocation(),
      DebugColor, false);
  }
}

void AInteractiveObject::ClearInflunces()
{
  for (AInteractiveObject* DependingObject : InfluencesList)
  {
    DependingObject->RemoveDependenceFrom(this);
  }
}

void AInteractiveObject::ClearDependencies()
{
  for (AInteractiveObject* InfluencingObject : DependenciesList)
  {
    RemoveDependenceFrom(InfluencingObject);
  }
}

std::shared_ptr<Node<AInteractiveObject>>& AInteractiveObject::GetNode()
{
  return NodeForTags;
}

TArray<int32>& AInteractiveObject::GetTags()
{
  return TagsForTypizationSearch;
}