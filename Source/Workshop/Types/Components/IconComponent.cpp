// Fill out your copyright notice in the Description page of Project Settings.

#include "IconComponent.h"


UIconComponent::UIconComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

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

void UIconComponent::Initialize()
{
  SpriteOfIcon->SetRelativeScale3D(FVector(
    IconProperties.Scale, IconProperties.Scale, IconProperties.Scale));

  SetAvailability(false);
  Show();
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

void UIconComponent::SetAvailability(bool bNewAvailability)
{
  bIsAvailable = bNewAvailability;
  if (bIsAvailable)
  {
    SpriteOfIcon->SetFlipbook(IconProperties.SpriteIfAvailable);
  }
  else
  {
    SpriteOfIcon->SetFlipbook(IconProperties.SpriteIfUnavailable);
  }
}

bool UIconComponent::IsAvailable() const
{
  return bIsAvailable;
}

bool UIconComponent::IsShown() const
{
  return bIsShown;
}
