// ...

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/SizeBox.h"
#include "Components/ScaleBox.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "AbilitySlot.generated.h"

UCLASS()
class WORKSHOP_API UAbilitySlot : public UUserWidget
{
  GENERATED_BODY()

private:
  UPROPERTY() UImage* AbilityIcon;
  UPROPERTY() USizeBox* IconSizeBox;

public:
  void SetMaxSize(float IconMaxSize, float IconMinSize);

  FVector2D GetIconCenterPosition() const;

  virtual void NativeConstruct() override;
};
