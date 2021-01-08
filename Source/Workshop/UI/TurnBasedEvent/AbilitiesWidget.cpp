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

void UAbilitiesWidget::FillBarSlots(const TArray<FBar>& Bars)
{
  size_t SlotsIndex = 0;
  for (const FBar& Bar : Bars)
  {
    if (SlotsIndex == BarsSlots.Num())
    {
      AddBarSlot();
    }

    BarsSlots[SlotsIndex]->SetBar(Bar);
    BarsLayout->InvalidateLayoutAndVolatility();

    SlotsIndex++;
  }

  while (BarsSlots.Num() > Bars.Num())
  {
    BarsSlots[BarsSlots.Num() - 1]->RemoveFromParent();
    BarsSlots.Pop();
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

  // Add DEFAULT_SLOTS_COUNT to vertical box; 
  for (size_t SlotsIndex = 0; SlotsIndex < DEFAULT_SLOTS_COUNT; SlotsIndex++)
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

void UAbilitiesWidget::AddBarSlot()
{
  UMixedProgressBar* NewBarSlot = WidgetTree->ConstructWidget<UMixedProgressBar>(BarSlotClass);
  BarsSlots.Add(NewBarSlot);

  UVerticalBoxSlot* NewSlot = BarsLayout->AddChildToVerticalBox(NewBarSlot);
  NewSlot->SetSize(FSlateChildSize(ESlateSizeRule::Type::Fill));
  BarsLayout->InvalidateLayoutAndVolatility();
}

void UAbilitiesWidget::RemoveAbilitySlot()
{
  VerticalBoxSlots[VerticalBoxSlots.Num() - 1]->RemoveFromParent();
  VerticalBoxSlots.Pop();
}

void UAbilitiesWidget::HideAbilitySlots()
{
  for (UAbilitySlot* SlotToHide : VerticalBoxSlots)
  {
    SlotToHide->SetVisibility(ESlateVisibility::Hidden);
  }
}

void UAbilitiesWidget::HideBarsSlots()
{
  for (UMixedProgressBar* SlotToHide : BarsSlots)
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

void UAbilitiesWidget::ShowBarsSlots()
{
  for (UMixedProgressBar* SlotToHide : BarsSlots)
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
