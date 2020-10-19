// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveObject.h"

// Sets default values
AInteractiveObject::AInteractiveObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MainScene"));;

  CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
  CollisionBox->SetupAttachment(RootComponent);
  CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_EngineTraceChannel1, ECollisionResponse::ECR_Block);

  InteractivityIcon = CreateDefaultSubobject<UIconComponent>(TEXT("TouchIcon"));
  InteractivityIcon->SetupAttachment(RootComponent);
}

void AInteractiveObject::OnConstruction(const FTransform & Transform)
{
  Super::OnConstruction(Transform);

  InteractivityIcon->Initialize();
  RefreshInteractive();
}

void AInteractiveObject::PostInitProperties()
{
  Super::PostInitProperties();
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
  if (!InfluencesSet.Find(TargetObject))
  {
    InfluencesSet.Add(TargetObject);
    TargetObject->DependenciesSet.Add(this);
  }
  else
  {
    UE_LOG(LogTemp, Warning, TEXT("Addition failed, objects are already connected"));
  }
}

void AInteractiveObject::RemoveDependenceFrom(AInteractiveObject * TargetObject)
{
  if (DependenciesSet.Find(this))
  {
    TargetObject->InfluencesSet.Remove(this);
    DependenciesSet.Remove(TargetObject);
  }
  else
  {
    UE_LOG(LogTemp, Warning, TEXT("Removal failed, objects are not connected"));
  }
}

#if WITH_EDITOR
void AInteractiveObject::ShowInfluences() const
{
  // Show what objects are influenced by this object
  for (AInteractiveObject* DependentObject : InfluencesSet)
  {
    DrawDebugLine(GetWorld(), GetActorLocation(), DependentObject->GetActorLocation(),
      DebugColor, false, DebugTime);
  }
}
#endif

void AInteractiveObject::ClearInflunces()
{
  for (AInteractiveObject* DependingObject : InfluencesSet)
  {
    DependingObject->DependenciesSet.Remove(this);
  }
  InfluencesSet.Empty();
}

void AInteractiveObject::ClearDependencies()
{
  for (AInteractiveObject* InfluencingObject : DependenciesSet)
  {
    InfluencingObject->InfluencesSet.Remove(this);
  }
  DependenciesSet.Empty();
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

int32 AInteractiveObject::GetInteractiveType() const
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
  MainManager->CentralObject->AddInfluenceOn(this);

  InteractivityIcon->SetIconState(EIconState::ChosenTarget);
}

void AInteractiveObject::UnpickedAsTarget()
{
  check(MainManager != nullptr);
  RemoveDependenceFrom(MainManager->CentralObject);

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

void AInteractiveObject::ResolveAccumulatedEffects(ETurnPhase TurnPhase)
{
  for (size_t EffectIndex = 0; EffectIndex < InteractiveDataCore.AccumulatedEffects[TurnPhase].Num(); EffectIndex++)
  {
    UEffectData* ChosenEffect = InteractiveDataCore.AccumulatedEffects[TurnPhase][EffectIndex];

    ChosenEffect->ResolveOn(this);
    ChosenEffect->DecreaseDuration();

    // Remove effect if it is no longer present
    if (!ChosenEffect->Duration)
    {
      InteractiveDataCore.AccumulatedEffects[TurnPhase].Swap(EffectIndex, InteractiveDataCore.AccumulatedEffects[TurnPhase].Num() - 1);
      InteractiveDataCore.AccumulatedEffects[TurnPhase].Pop();
    }
  }
}

void AInteractiveObject::RefreshInteractive()
{
  // Change Y-Extent
  FVector ScaledBoxExtent(CollisionBox->GetScaledBoxExtent());
  CollisionBox->SetBoxExtent(FVector(ScaledBoxExtent[0], CollisionBoxWidth, ScaledBoxExtent[2]));

  // Set Y-order
  FVector BoxLocation = CollisionBox->GetRelativeLocation();
  CollisionBox->SetRelativeLocation(FVector(
    BoxLocation[0], MainSpriteYOrder, BoxLocation[2]));
}
