// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveObject.h"
#include "Workshop/Types/Bar.h"

AInteractiveObject::AInteractiveObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  InteractiveDataCore.InteractiveType = 0;

  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MainScene"));;

  CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
  CollisionBox->SetupAttachment(RootComponent);
  CollisionBox->SetCollisionProfileName(TEXT("Interactive"));

  InteractivityIcon = CreateDefaultSubobject<UIconComponent>(TEXT("TouchIcon"));
  InteractivityIcon->SetupAttachment(RootComponent);
}

void AInteractiveObject::OnConstruction(const FTransform & Transform)
{
  Super::OnConstruction(Transform);

  InteractivityIcon->Initialize();
  BuildInteractive();
}

void AInteractiveObject::PostInitializeComponents()
{
  Super::PostInitializeComponents();

  for (FBar& Stat : InteractiveDataCore.Stats)
  {
    Stat.ResetBar();
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

bool AInteractiveObject::AddInfluenceOn(AInteractiveObject* TargetObject)
{
  if (!InfluencesOn.Find(TargetObject))
  {
    InfluencesOn.Add(TargetObject);
    TargetObject->DependsOn.Add(this);
    return true;
  }

  return false;
}

bool AInteractiveObject::RemoveInfluenceOn(AInteractiveObject * TargetObject)
{
  if (InfluencesOn.Find(TargetObject))
  {
    InfluencesOn.Remove(TargetObject);
    TargetObject->DependsOn.Remove(this);
    return true;
  }

  return false;
}

#if WITH_EDITOR
void AInteractiveObject::ShowInfluences() const
{
  // Show what objects are influenced by this object
  for (AInteractiveObject* DependentObject : InfluencesOn)
  {
    DrawDebugLine(GetWorld(), GetActorLocation(), DependentObject->GetActorLocation(),
      DEBUG_COLOR, false, DEBUG_TIME);
  }
}
#endif

void AInteractiveObject::ClearInflunces()
{
  for (AInteractiveObject* DependentObject : InfluencesOn)
  {
    DependentObject->DependsOn.Remove(this);
  }
  InfluencesOn.Empty();
}

void AInteractiveObject::ClearDependencies()
{
  for (AInteractiveObject* InfluencingObject : DependsOn)
  {
    InfluencingObject->InfluencesOn.Remove(this);
  }
  DependsOn.Empty();
}

std::shared_ptr<Node<AInteractiveObject>>& AInteractiveObject::GetNodeForCT()
{
  return NodeForCT;
}

const TArray<int32>* AInteractiveObject::GetCTs() const
{
  return &InteractiveDataCore.CTsOfObject;
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

uint8 AInteractiveObject::GetInteractiveType() const
{
  return InteractiveDataCore.InteractiveType;
}

void AInteractiveObject::PickedAsCentral()
{
  check(MainManager != nullptr);
  MainManager->CentralObject = this;

  InteractivityIcon->SetIconState(EIconState::CentralObject);
  InteractivityIcon->Show();
}

void AInteractiveObject::UnpickedAsCentral()
{
  check(MainManager != nullptr);
  MainManager->CentralObject = nullptr;

  InteractivityIcon->Hide();
}

void AInteractiveObject::PickedAsTarget()
{
  check(MainManager != nullptr);
  if (!MainManager->CentralObject->AddInfluenceOn(this))
  {
    return;
  }

  InteractivityIcon->SetIconState(EIconState::ChosenTarget);
}

void AInteractiveObject::UnpickedAsTarget()
{
  check(MainManager != nullptr);
  MainManager->CentralObject->RemoveInfluenceOn(this);

  InteractivityIcon->SetIconState(EIconState::AvailableTarget);
}

bool AInteractiveObject::IsCentralInManager() const
{
  check(MainManager);
  return MainManager->CentralObject == this;
}

void AInteractiveObject::Pick()
{
  if (MainManager == nullptr)
  {
    UE_LOG(LogTemp, Error, TEXT("Object isn't connected to Manager!"));
    return;
  }

  if (!MainManager->HasCentralObject())
  {
    PickedAsCentral();
    return;
  }

  if (IsCentralInManager())
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
  
  if (InteractivityIcon->GetIconState() == EIconState::AvailableTarget)
  {
    PickedAsTarget();
    return;
  }
  else
  {
    UnpickedAsTarget();
  }
}

void AInteractiveObject::BuildInteractive()
{
  // Change Y-Extent
  FVector ScaledBoxExtent(CollisionBox->GetScaledBoxExtent());
  CollisionBox->SetBoxExtent(FVector(ScaledBoxExtent[0], COLLISION_BOX_WIDTH, ScaledBoxExtent[2]));

  // Set Y-order
  FVector BoxLocation = CollisionBox->GetRelativeLocation();
  CollisionBox->SetRelativeLocation(FVector(
    BoxLocation[0], SPRITE_Y_ORDER, BoxLocation[2]));
}

void AInteractiveObject::UpdateExhaust()
{

}

void AInteractiveObject::PlayAnimation(int32 AnimationId, bool bWaitUntilEnds)
{

}

void AInteractiveObject::OnExhausted_Implementation()
{

}

void AInteractiveObject::SetManager(ARegistrationManager* NewManager)
{
  MainManager = NewManager;
}

void AInteractiveObject::Sleep()
{
  InteractivityIcon->Hide();
}

void AInteractiveObject::AwakeBy(AInteractiveObject* Object)
{
  InteractivityIcon->Show();

  if (DependsOn.Find(Object))
  {
    InteractivityIcon->SetIconState(EIconState::ChosenTarget);
  }
  else
  {
    InteractivityIcon->SetIconState(EIconState::AvailableTarget);
  }
}

void AInteractiveObject::SetInteractiveType(EInteractiveType NewType)
{
  InteractiveDataCore.InteractiveType = static_cast<uint8>(NewType);
}