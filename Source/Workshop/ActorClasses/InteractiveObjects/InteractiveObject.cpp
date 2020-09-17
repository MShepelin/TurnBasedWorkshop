// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveObject.h"


// Sets default values
AInteractiveObject::AInteractiveObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MainScene"));;

  InteractivityIcon = CreateDefaultSubobject<UIconComponent>(TEXT("TouchIcon"));
  InteractivityIcon->SetupAttachment(RootComponent);
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
    UE_LOG(LogTemp, Error, TEXT("Incorrect object stats: a name stat is needed!"));
  }
}

void AInteractiveObject::BeginPlay()
{
	Super::BeginPlay();

  InteractivityIcon->Hide();
}

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

/* REMAKE
FString AInteractiveObject::GatherInformation() const
{
  if (!MainManager)
  {
    UE_LOG(LogTemp, Error, TEXT("Can't gather information without correct manager!"));
    return "";
  }

  return "";
  //return StringStats[ObjectNameStatID].ToString() + "\n";
}
*/

void AInteractiveObject::ShowInfluences() const
{
#if WITH_EDITOR
  // Show what objects are influenced by this object
  for (AInteractiveObject* DependentObject : InfluencesArray)
  {
    DrawDebugLine(GetWorld(), GetActorLocation(), DependentObject->GetActorLocation(),
      DebugColor, false, DebugTime);
  }
#endif
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

void AInteractiveObject::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  if (NodeForCT)
  {
    NodeForCT.reset();
  }
}

int32 AInteractiveObject::GetInteractiveType() const
{
  return InteractiveType;
}

FName AInteractiveObject::GetInteractiveObjectName() const
{
  return StringStats[ObjectNameStatID];
}

void AInteractiveObject::SetTurn(ETurnPhase TurnPhase)
{

}

void AInteractiveObject::RemoveEffectByIndex(int32 EffectIndex) //???? add inline?
{
  AccumulatedEffects.Swap(EffectIndex, AccumulatedEffects.Num() - 1);
  AccumulatedEffects.Pop();
}

void AInteractiveObject::ConnectToManager(ARegistrationManager* NewManager)
{
  if (MainManager)
  {
    MainManager->CTsSystem->RemoveObject(this);

    if (MainManager->CentralObject == this)
    {
      UnpickedAsCentral();
    }
  }

  MainManager = NewManager;

  MainManager->CTsSystem->AddObject(this);
}

void AInteractiveObject::PickedAsCentral()
{
  check(MainManager != nullptr);

  MainManager->CentralObject = this;

  //++++ add info to hud
}

void AInteractiveObject::UnpickedAsCentral()
{
  check(MainManager != nullptr);

  MainManager->CentralObject = nullptr;

  //++++ remove info from hud
}

void AInteractiveObject::PickedAsTarget()
{
  check(MainManager != nullptr);

  MainManager->CentralObject->AddInfluenceOn(this);

  InteractivityIcon->SetAvailability(false);
}

void AInteractiveObject::UnpickedAsTarget()
{
  check(MainManager != nullptr);

  RemoveDependenceFrom(MainManager->CentralObject);

  InteractivityIcon->SetAvailability(true);
}

void AInteractiveObject::AwakeDependingOnInfluence(TArray<AInteractiveObject*>& Objects)
{
  for (AInteractiveObject* Object : Objects)
  {
    //???? change every used in this case tarray to tarray with TInlineAllocator
    MainManager->AwakenObjects.Add(Object);

    Object->InteractivityIcon->Show();

    if (InfluencesArray.Find(Object))
    {
      Object->InteractivityIcon->SetAvailability(false);
    }
    else
    {
      Object->InteractivityIcon->SetAvailability(true);
    }
  }
}

void AInteractiveObject::PutToSleepManagedObjects(ARegistrationManager* Manager)
{
  TArray<AInteractiveObject*, TInlineAllocator<AverageManagedObjects>>& AwakenObjects = Manager->AwakenObjects;

  while (AwakenObjects.Num())
  {
    AwakenObjects.Pop()->InteractivityIcon->Hide();
  }
}

TArray<FString> AInteractiveObject::GetCTsNamesOfObject() const
{
  check(MainManager != nullptr);

  TArray<FString> CTsNames;

  for (int32 ObjectCT : *GetCTs())
  {
    CTsNames.Add(MainManager->GetCTName(ObjectCT));
  }

  return CTsNames;
}

bool AInteractiveObject::IsCentral() const
{
  check(MainManager);
  return MainManager->CentralObject == this;
}

void AInteractiveObject::Pick()
{
  if (!MainManager->HasCentralObject())
  {
    PickedAsCentral();
    return;
  }

  if (IsCentral())
  {
    UnpickedAsCentral();
    return;
  }

  if (!InteractivityIcon->IsShown())
  {
    MainManager->CentralObject->UnpickedAsCentral();
    PickedAsCentral();
    return;
  }

  if (InteractivityIcon->IsAvailable())
  {
    PickedAsTarget();
    return;
  }
  else
  {
    UnpickedAsTarget();
  }
}
