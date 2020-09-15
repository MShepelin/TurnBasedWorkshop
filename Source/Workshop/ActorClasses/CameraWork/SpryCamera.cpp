//...

#include "SpryCamera.h"
#include "../InteractiveObjects/InteractiveCharacter.h"


ASpryCamera::ASpryCamera()
{
  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CameraScene"));

  Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
  Camera->SetupAttachment(RootComponent);

  HiddenLocation = FVector(0, 0, 0);
}

void ASpryCamera::OnCharacterResolvesAbility(AInteractiveCharacter* CurrentCharacter)
{
  // apply movement
}

FVector ASpryCamera::GetHiddenLocation() const
{
  return HiddenLocation;
}
