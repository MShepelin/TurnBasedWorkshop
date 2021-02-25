// Copyright © Dmitriy Shepelin 2021. MIT License.

#include "AbilitySlot.h"
#include "Blueprint/WidgetTree.h"
#include "Workshop/Types/Nonblueprintable/GameConstants.h"
#include "Workshop/WorkshopGameModeBase.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveAbility.h"

void UAbilitySlot::AbilityClicked()
{
  if (!ChosenAbility || !bIsControlledByPlayer)
  {
    return;
  }

  ChosenAbility->CenterInCharacterOwner();
  //++++ add usage of EmptySlot texture
}

void UAbilitySlot::SetChosenAbility(AInteractiveAbility* NewAbility, bool bIsControlled)
{
  bIsControlledByPlayer = bIsControlled;
  ChosenAbility = NewAbility;
  AbilityIcon->SetBrushFromTexture(ChosenAbility->GetIconUI());

  FString CollectedEffectsInfo = "";

  AWorkshopGameModeBase* GameMode = Cast<AWorkshopGameModeBase>(GetWorld()->GetAuthGameMode());
  if (!GameMode)
  {
    UE_LOG(LogTemp, Error, TEXT("AbilitySlot tried to SetChosenAbility but GameMode isn't inherited from WorkshopGameModeBase"));
    return;
  }

  for (int32 CTToAffect : NewAbility->AbilityDataCore.CTsToAffect)
  {
    FCTData CTData = GameMode->GetCTData(CTToAffect);

    CollectedEffectsInfo += \
      "<" + CTData.TextStyle.ToString() + ">" + CTData.ShownName.ToString() + "</> ";
  }

  if (NewAbility->AbilityDataCore.CTsToAffect.Num())
  {
    CollectedEffectsInfo += "gets\n";
  }
  else
  {
    CollectedEffectsInfo += TEXT("Any target gets\n");
  }

  for (FEffectData Effect : NewAbility->AbilityDataCore.UsedEffects)
  {
    if (Effect.bIsBonusEffect)
    {
      CollectedEffectsInfo += "<BonusEffect>"; //++++ define BonusEffect
    }
    CollectedEffectsInfo += Effect.GetInfoString(GameMode);
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
  AbilityButton->OnClicked.AddDynamic(this, &UAbilitySlot::AbilityClicked);
}
