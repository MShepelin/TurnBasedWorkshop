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

  bIsPossesedByPlayer = false;
}

FVector ASpryCamera::GetHiddenLocation() const
{
  return UKismetMathLibrary::TransformLocation(GetActorTransform(), HiddenLocation);
}

void ASpryCamera::PossessedBy(AController * NewController)
{
  Super::PossessedBy(NewController);

  FVector CameraDirection = Camera->GetComponentRotation().Vector();
  HiddenLocation = Camera->GetComponentLocation() - CameraDirection * MAX_OBJECT_SIZE;

  PlayerController = Cast<APlayerController>(NewController);

  if (PlayerController)
  {
    bIsPossesedByPlayer = true;
  }
}

void ASpryCamera::UnPossessed()
{
  Super::UnPossessed();
  bIsPossesedByPlayer = false;
  PlayerController = nullptr;
}

FVector ASpryCamera::GetCameraDirection() const
{
  if (!bIsPossesedByPlayer)
  {
    return Camera->GetComponentRotation().Vector();
  }

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

void ASpryCamera::PlayerScrolledWheel(float ScrollDelta)
{
  WidgetInteraction->ScrollWheel(ScrollDelta);
}

FVector ASpryCamera::GetCameraLocation() const
{
  return Camera->GetComponentLocation();
}
