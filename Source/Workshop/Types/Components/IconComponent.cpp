// Fill out your copyright notice in the Description page of Project Settings.

#include "IconComponent.h"


// Sets default values for this component's properties
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

// Called when the game starts
void UIconComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UIconComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UIconComponent::Initialize()
{
  bIsAvailable = false;

  if (IconPropertiesClass && !IconProperties)
  {
    IconProperties = NewObject<UIconData>(this, IconPropertiesClass, TEXT("IconData"));

    SpriteOfIcon->SetFlipbook(IconProperties->SpriteIfUnavailable);
    SpriteOfIcon->SetRelativeScale3D(FVector(
      IconProperties->Scale, IconProperties->Scale, IconProperties->Scale));
  }
  else
  {
    //Unreal engine does some magic, don't interrupt please
    //UE_LOG(LogTemp, Warning, TEXT("Initialize failed!"));
  }

  Show();
}

void UIconComponent::Show()
{
  SpriteOfIcon->SetVisibility(true, true);
}

void UIconComponent::Hide()
{
  SpriteOfIcon->SetVisibility(false, true);
}

void UIconComponent::SetAvailability(bool bNewAvailability)
{
  bIsAvailable = bNewAvailability;
  if (bIsAvailable)
  {
    SpriteOfIcon->SetFlipbook(IconProperties->SpriteIfAvailable);
  }
  else
  {
    SpriteOfIcon->SetFlipbook(IconProperties->SpriteIfUnavailable);
  }
}

bool UIconComponent::IsAvailable()
{
  return bIsAvailable;
}
