// ...

#include "AbilitySlot.h"
#include "Blueprint/WidgetTree.h"
#include "Workshop/Types/Nonblueprintable/GameConstants.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveAbility.h"

void UAbilitySlot::AbilityClicked()
{
  if (!ChosenAbility)
  {
    return;
  }

  ChosenAbility->Pick();
  ChosenAbility->CenterInOwner();
}

void UAbilitySlot::SetMaxSize(float IconMaxSize, float IconMinSize)
{
  IconSizeBox->MinDesiredWidth = IconMinSize;
  IconSizeBox->MinDesiredHeight = IconMinSize;
  IconSizeBox->MaxDesiredWidth = IconMaxSize;
  IconSizeBox->MaxDesiredHeight = IconMaxSize;
}

void UAbilitySlot::SetChosenAbility(AInteractiveAbility* NewAbility)
{
  ChosenAbility = NewAbility;
}

void UAbilitySlot::NativeConstruct()
{
  UPanelWidget* RootWidget = Cast<UPanelWidget>(GetRootWidget());

  if (!RootWidget)
  {
    return;
  }

  //ScaleBox = WidgetTree->ConstructWidget<UScaleBox>(UScaleBox::StaticClass(), TEXT("ScaleBox"));
  //ScaleBox->StretchDirection = EStretchDirection::Type::DownOnly;
  //RootWidget->AddChild(ScaleBox);
  
  //IconSizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("SizeBox"));
  //IconSizeBox->MinDesiredWidth = MinIconSize;
  //IconSizeBox->MinDesiredHeight = MinIconSize;
  //IconSizeBox->MaxDesiredWidth = MaxIconSize;
  //IconSizeBox->MaxDesiredHeight = MaxIconSize;
  //ScaleBox->AddChild(IconSizeBox);

  //AbilityButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("AbilityButton"));
 // AbilityButton->ColorAndOpacity = FLinearColor(0, 0, 0, 0);
  if (AbilityButton)
  {
    AbilityButton->OnClicked.AddDynamic(this, &UAbilitySlot::AbilityClicked);
  }
  //IconSizeBox->AddChild(AbilityButton);
 
  //AbilityIcon = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("AbilityIcon"));
  //AbilityButton->AddChild(AbilityIcon);

  Super::NativeConstruct();
}
