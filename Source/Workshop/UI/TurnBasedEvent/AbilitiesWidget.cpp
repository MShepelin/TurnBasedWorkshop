// ...

#include "AbilitiesWidget.h"
#include "../AbilitySlot.h"
#include "Workshop/Types/Nonblueprintable/GameConstants.h"
#include "Components/SlateWrapperTypes.h"
#include "TimerManager.h"

void UAbilitiesWidget::FillAbilitySlots(const TArray<AInteractiveAbility*>& Abilities, ARegistrationManager* UsedManager)
{
  check(AbilitySlotClass != nullptr);

  size_t SlotsIndex = 0;
  for (AInteractiveAbility* Ability : Abilities)
  {
    if (SlotsIndex == VerticalBoxSlots.Num())
    {
      AddAbilitySlot();
    }

    VerticalBoxSlots[SlotsIndex]->SetChosenAbility(Ability, UsedManager);
    AbilitiesLayout->InvalidateLayoutAndVolatility();
    
    SlotsIndex++;
  }

  while (VerticalBoxSlots.Num() > Abilities.Num())
  {
    RemoveAbilitySlot();
  }
}

void UAbilitiesWidget::NativePreConstruct()
{
  if (!AbilitySlotClass)
  {
    UE_LOG(LogTemp, Error, TEXT("AbilitySlotClass must be set in AbilitiesWidget!"));
    Super::NativePreConstruct();
    return;
  }

  // Add DefaultNumberOfSlots to vertical box; 
  for (size_t SlotsIndex = 0; SlotsIndex < DefaultNumberOfSlots; SlotsIndex++)
  {
    AddAbilitySlot();
  }

  Super::NativePreConstruct();

  //++++ AbilitiesLayout -> set standart indent
}

void UAbilitiesWidget::AddAbilitySlot()
{
  UAbilitySlot* NewAbilitySlot = WidgetTree->ConstructWidget<UAbilitySlot>(AbilitySlotClass);
  VerticalBoxSlots.Add(NewAbilitySlot);

  UVerticalBoxSlot* NewSlot = AbilitiesLayout->AddChildToVerticalBox(NewAbilitySlot);
  NewSlot->SetSize(FSlateChildSize(ESlateSizeRule::Type::Fill));
  AbilitiesLayout->InvalidateLayoutAndVolatility();
}

void UAbilitiesWidget::RemoveAbilitySlot()
{
  VerticalBoxSlots[VerticalBoxSlots.Num() - 1]->RemoveFromParent();
  VerticalBoxSlots.Pop();
}

void UAbilitiesWidget::HideAbilitySlots()
{
  for (size_t NewSlotIndex = VerticalBoxSlots.Num(); NewSlotIndex <= DefaultNumberOfSlots; NewSlotIndex++)
  {
    AddAbilitySlot();
  }

  for (UAbilitySlot* SlotToHide : VerticalBoxSlots)
  {
    SlotToHide->SetVisibility(ESlateVisibility::Hidden);
  }
}

void UAbilitiesWidget::ShowAbilitySlots()
{
  for (UAbilitySlot* SlotToHide : VerticalBoxSlots)
  {
    SlotToHide->SetVisibility(ESlateVisibility::Visible);
  }
}

void UAbilitiesWidget::FailToInteract()
{
  MassageText->SetText(FText::FromString("<BigText> Fail To Interact! </>"));
}

void UAbilitiesWidget::ShowBasicText()
{
  MassageText->SetText(FText::FromString("<BigText> Info </>"));
}
