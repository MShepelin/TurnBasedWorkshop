//...

#include "SpryCamera.h"
#include "../InteractiveObjects/InteractiveCharacter.h"
#include "Workshop/Types/Nonblueprintable/GameConstants.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

ASpryCamera::ASpryCamera()
{
  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CameraScene"));

  Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
  Camera->SetupAttachment(RootComponent);

  WidgetInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteraction"));
  WidgetInteraction->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
  WidgetInteraction->InteractionSource = EWidgetInteractionSource::Mouse;
}

/*
void ASpryCamera::OnCharacterResolvesAbility_Implementation(AInteractiveCharacter* CurrentCharacter)
{
  // apply movement
}
*/

FVector ASpryCamera::GetHiddenLocation() const
{
  return UKismetMathLibrary::TransformLocation(GetActorTransform(), HiddenLocation);
}

void ASpryCamera::OnConstruction(const FTransform & Transform)
{
  Super::OnConstruction(Transform);

  FVector CameraDirection = Camera->GetRelativeRotation().Vector();
  HiddenLocation = Camera->GetRelativeLocation() - CameraDirection*MaxActorSize;
}

void ASpryCamera::PossessedBy(AController * NewController)
{
  Super::PossessedBy(NewController);
  PlayerController = Cast<APlayerController>(NewController);

  if (PlayerController)
  {
    bIsPossesed = true;
  }
}

void ASpryCamera::UnPossessed()
{
  Super::UnPossessed();
  bIsPossesed = false;
  PlayerController = nullptr;
}

FVector ASpryCamera::GetCameraDirection() const
{
  FVector WorldLocation;
  FVector WorldDirection;
  PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

  return WorldLocation - WidgetInteraction->GetComponentLocation();
}

void ASpryCamera::PlayerPressedClick()
{
  WidgetInteraction->PressPointerKey(FKey("LeftMouseButton"));
}

void ASpryCamera::PlayerReleasedClick()
{
  WidgetInteraction->ReleasePointerKey(FKey("LeftMouseButton"));
}

FVector ASpryCamera::GetCameraLocation() const
{
  return Camera->GetComponentLocation();
}
