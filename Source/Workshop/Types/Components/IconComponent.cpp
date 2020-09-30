// Fill out your copyright notice in the Description page of Project Settings.

#include "IconComponent.h"


UIconComponent::UIconComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

  IconState = EIconState::AvailableTarget;

  if (!IsTemplate())
  {
    SpriteOfIcon = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("TouchIconComponent"));
    SpriteOfIcon->SetupAttachment(this);
  }
}

void UIconComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UIconComponent::PostInitProperties()
{
  Super::PostInitProperties();
}

void UIconComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UIconComponent::SetIconState(EIconState NewIconState)
{
  IconState = NewIconState;
  switch (IconState)
  {
  case EIconState::AvailableTarget:
    SpriteOfIcon->SetFlipbook(IconProperties.SpriteIfAvailable);
    break;
  case EIconState::ChosenTarget:
    SpriteOfIcon->SetFlipbook(IconProperties.SpriteIfChosen);
    break;
  case EIconState::CentralObject:
    SpriteOfIcon->SetFlipbook(IconProperties.SpriteIfCentral);
    break;
  default:
    break;
  }
}

EIconState UIconComponent::GetIconState() const
{
  return IconState;
}

void UIconComponent::Initialize()
{
  if (!IsTemplate())
  {
    SpriteOfIcon->SetRelativeScale3D(FVector(
      IconProperties.Scale, IconProperties.Scale, IconProperties.Scale));

    SetIconState(EIconState::AvailableTarget);
    Show();
  }
}

void UIconComponent::Show()
{
  bIsShown = true;
  SpriteOfIcon->SetVisibility(true, true);
}

void UIconComponent::Hide()
{
  bIsShown = false;
  SpriteOfIcon->SetVisibility(false, true);
}

bool UIconComponent::IsShown() const
{
  return bIsShown;
}
