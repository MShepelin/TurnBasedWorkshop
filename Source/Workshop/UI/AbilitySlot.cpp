// ...

#include "AbilitySlot.h"
#include "Blueprint/WidgetTree.h"
#include "Workshop/Types/Nonblueprintable/GameConstants.h"

FVector2D UAbilitySlot::GetIconCenterPosition() const
{
  return FVector2D();
}

void UAbilitySlot::SetMaxSize(float IconMaxSize, float IconMinSize)
{
  IconSizeBox->MinDesiredWidth = IconMinSize;
  IconSizeBox->MinDesiredHeight = IconMinSize;
  IconSizeBox->MaxDesiredWidth = IconMaxSize;
  IconSizeBox->MaxDesiredHeight = IconMaxSize;
}

void UAbilitySlot::NativeConstruct()
{
  Super::NativeConstruct();
  
  UPanelWidget* RootWidget = Cast<UPanelWidget>(GetRootWidget());

  UScaleBox* ScaleBox = WidgetTree->ConstructWidget<UScaleBox>(UScaleBox::StaticClass(), TEXT("ScaleBox"));
  ScaleBox->StretchDirection = EStretchDirection::Type::DownOnly;
  RootWidget->AddChild(ScaleBox);

  IconSizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("SizeBox"));
  IconSizeBox->MinDesiredWidth = MinIconSize;
  IconSizeBox->MinDesiredHeight = MinIconSize;
  IconSizeBox->MaxDesiredWidth = MaxIconSize;
  IconSizeBox->MaxDesiredHeight = MaxIconSize;
  ScaleBox->AddChild(IconSizeBox);

  AbilityIcon = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("AbilityIcon"));
  IconSizeBox->AddChild(AbilityIcon);
}
