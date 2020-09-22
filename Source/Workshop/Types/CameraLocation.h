//...

#include "CoreMinimal.h"
#include "CameraLocation.generated.h"

USTRUCT()
struct FCameraLocation
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere) FVector CameraSelfLocation;
  UPROPERTY(EditAnywhere) TArray<FVector> ParentLocations;
};
