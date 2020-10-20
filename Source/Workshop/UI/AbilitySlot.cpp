// ...

#include "AbilitySlot.h"
#include "Blueprint/WidgetTree.h"
#include "Workshop/Types/Nonblueprintable/GameConstants.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveAbility.h"

void UAbilitySlot::AbilityClicked()
{
  UE_LOG(LogTemp, Warning, TEXT("Click started"));
  if (!ChosenAbility)
  {
    return;
  }

  UE_LOG(LogTemp, Warning, TEXT("Click coused centring"));

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

  FString CollectedEffectsInfo = "";

  for (UEffectData* Effect : NewAbility->AbilityDataCore.UsedEffects)
  {
    CollectedEffectsInfo += Effect->GetInfoString() + "\n";
  }
  
  EffectsInfo->SetText(FText::FromString(CollectedEffectsInfo));
}

void UAbilitySlot::NativePreConstruct()
{
  Super::NativePreConstruct();

  AbilityButton->SetBackgroundColor(FLinearColor(0, 0, 0, 0));
  AbilityButton->OnPressed.AddDynamic(this, &UAbilitySlot::AbilityClicked);
}
