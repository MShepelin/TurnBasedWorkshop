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

  if (!StringStats.Find(ObjectNameStatID))
  {
    UE_LOG(LogTemp, Error, TEXT("Incorrect object stats!"));
  }
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
  if (!InfluencesArray.Find(TargetObject))
  {
    InfluencesArray.Add(TargetObject);
    TargetObject->DependenciesArray.Add(this);
  }
  else
  {
    UE_LOG(LogTemp, Warning, TEXT("Addiction failed, objects are already connected"));
  }
}


void AInteractiveObject::RemoveDependenceFrom(AInteractiveObject * TargetObject)
{
  if (DependenciesArray.Find(this))
  {
    TargetObject->InfluencesArray.Remove(this);
    DependenciesArray.Remove(TargetObject);
  }
  else
  {
    UE_LOG(LogTemp, Warning, TEXT("Removal failed, objects are not connected"));
  }
}


FString AInteractiveObject::GatherInformation() const
{
  if (!MainManager) // || IsValid(MainManager))
  {
    UE_LOG(LogTemp, Error, TEXT("Can't gather information without correct manager!"));
    return "";
  }

  return StringStats[ObjectNameStatID].ToString();
}


void AInteractiveObject::ShowInfluences() const
{
  // Show what objects are influenced by this object
  for (AInteractiveObject* DependentObject : InfluencesArray)
  {
    DrawDebugLine(GetWorld(), GetActorLocation(), DependentObject->GetActorLocation(),
      DebugColor, false, DebugTime);
  }
}


void AInteractiveObject::ClearInflunces()
{
  for (AInteractiveObject* DependingObject : InfluencesArray)
  {
    DependingObject->RemoveDependenceFrom(this);
  }
}


void AInteractiveObject::ClearDependencies()
{
  for (AInteractiveObject* InfluencingObject : DependenciesArray)
  {
    RemoveDependenceFrom(InfluencingObject);
  }
}


std::shared_ptr<Node<AInteractiveObject>>& AInteractiveObject::GetNodeForCT()
{
  return NodeForCT;
}


const TArray<int32>* AInteractiveObject::GetCTs() const
{
  return &CTsOfObject;
}


ARegistrationManager* AInteractiveObject::GetManager() const
{
  return MainManager;
}


void AInteractiveObject::SetManager(ARegistrationManager* NewManager)
{
  if (MainManager && NewManager)
  {
    UE_LOG(LogTemp, Error, TEXT("Manager is already set, this object should be disconnected first"));
    return;
  }

  MainManager = NewManager;
}


void AInteractiveObject::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  if (NodeForCT)
  {
    NodeForCT.reset();
  }
}
