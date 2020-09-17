//...

#include "SpryCamera.h"
#include "../InteractiveObjects/InteractiveCharacter.h"
#include "Workshop/Types/Nonblueprintable/GameConstants.h"


ASpryCamera::ASpryCamera()
{
  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CameraScene"));

  Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
  Camera->SetupAttachment(RootComponent);

  WidgetInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteraction"));

  WidgetInteraction->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
}

void ASpryCamera::OnCharacterResolvesAbility_Implementation(AInteractiveCharacter* CurrentCharacter)
{
  // apply movement
}

FVector ASpryCamera::GetHiddenLocation() const
{
  return HiddenLocation;
}

void ASpryCamera::OnConstruction(const FTransform & Transform)
{
  Super::OnConstruction(Transform);

  Camera->SetRelativeLocation(FVector(0, 0, 0));

  FVector CameraDirection = Camera->GetRelativeRotation().Vector();
  HiddenLocation = -CameraDirection*MaxActorSize;
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

void ASpryCamera::Tick(float DeltaSeconds)
{
  Super::Tick(DeltaSeconds);

  if (!bIsPossesed)
  {
    return;
  }

  FVector WorldLocation;
  FVector WorldDirection;

  PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

  WidgetInteraction->SetWorldRotation(WorldDirection.Rotation());
}

FVector ASpryCamera::GetCameraDirection() const
{
  return Camera->GetComponentRotation().Vector();
}

void ASpryCamera::PlayerPressedClick()
{
  WidgetInteraction->PressPointerKey(FKey("LeftMouseButton"));
}

void ASpryCamera::PlayerReleasedClick()
{
  WidgetInteraction->ReleasePointerKey(FKey("LeftMouseButton"));
}
