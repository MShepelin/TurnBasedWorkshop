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
 
  ChosenAbility->CenterInOwner();
  // ChosenAbility->Pick();
  //++++ add usage of EmptySlot texture
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
  AbilityIcon->SetBrushFromTexture(ChosenAbility->GetIconUI());
}

void UAbilitySlot::NativeConstruct()
{
  UPanelWidget* RootWidget = Cast<UPanelWidget>(GetRootWidget());

  if (!RootWidget)
  {
    return;
  }

  AbilityButton->OnClicked.AddDynamic(this, &UAbilitySlot::AbilityClicked);
  AbilityButton->SetColorAndOpacity(FLinearColor(0, 0, 0, 0)); // for now - only image matters
  Super::NativeConstruct();
}
