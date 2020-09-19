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
 
  ChosenAbility->CenterInCharacterOwner();
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

void UAbilitySlot::NativePreConstruct()
{
  Super::NativePreConstruct();

  AbilityButton->OnClicked.AddDynamic(this, &UAbilitySlot::AbilityClicked);
  AbilityButton->SetBackgroundColor(FLinearColor(0, 0, 0, 0.5)); // change to 0
}
