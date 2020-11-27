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

void UAbilitySlot::SetChosenAbility(AInteractiveAbility* NewAbility, ARegistrationManager* UsedManager)
{
  ChosenAbility = NewAbility;
  AbilityIcon->SetBrushFromTexture(ChosenAbility->GetIconUI());

  FString CollectedEffectsInfo = "";

  for (FEffectData Effect : NewAbility->AbilityDataCore.UsedEffects)
  {
    if (Effect.bIsBonusEffect)
    {
      CollectedEffectsInfo += "<BonusEffect>";
    }
    CollectedEffectsInfo += Effect.GetInfoString(UsedManager);
    if (Effect.bIsBonusEffect)
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
