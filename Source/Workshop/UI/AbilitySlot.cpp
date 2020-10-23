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
  //++++ add usage of EmptySlot texture
}

void UAbilitySlot::SetMaxSize(float IconMaxSize, float IconMinSize)
{
  IconSizeBox->MinDesiredWidth = IconMinSize;
  IconSizeBox->MinDesiredHeight = IconMinSize;
  IconSizeBox->MaxDesiredWidth = IconMaxSize;
  IconSizeBox->MaxDesiredHeight = IconMaxSize;
}

void UAbilitySlot::SetChosenAbility(AInteractiveAbility* NewAbility, ARegistrationManager* UsedManager)
{
  ChosenAbility = NewAbility;
  AbilityIcon->SetBrushFromTexture(ChosenAbility->GetIconUI());

  FString CollectedEffectsInfo = "";

  for (UEffectData* Effect : NewAbility->AbilityDataCore.UsedEffects)
  {
    if (Effect->bIsBonusEffect)
    {
      CollectedEffectsInfo += "<BonusEffect>";
    }
    CollectedEffectsInfo += Effect->GetInfoString(UsedManager);
    if (Effect->bIsBonusEffect)
    {
      CollectedEffectsInfo += "</>";
    }
    CollectedEffectsInfo += "\n";
  }
  
  EffectsInfo->SetText(FText::FromString(CollectedEffectsInfo));
}

void UAbilitySlot::NativePreConstruct()
{
  Super::NativePreConstruct();

  AbilityButton->SetBackgroundColor(FLinearColor(0, 0, 0, 0));
  AbilityButton->OnPressed.AddDynamic(this, &UAbilitySlot::AbilityClicked);
}
