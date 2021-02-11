// ...

#include "AbilitiesWidget.h"
#include "../AbilitySlot.h"
#include "Workshop/Types/Nonblueprintable/GameConstants.h"
#include "Components/SlateWrapperTypes.h"
#include "TimerManager.h"

void UAbilitiesWidget::FillAbilitySlots(const TArray<AInteractiveAbility*>& Abilities, bool bIsControlled)
{
  check(AbilitySlotClass != nullptr);

  size_t SlotsIndex = 0;
  for (AInteractiveAbility* Ability : Abilities)
  {
    if (SlotsIndex == VerticalBoxSlots.Num())
    {
      AddAbilitySlot();
    }

    VerticalBoxSlots[SlotsIndex]->SetChosenAbility(Ability, bIsControlled);
    
    SlotsIndex++;
  }

  while (VerticalBoxSlots.Num() > Abilities.Num())
  {
    RemoveAbilitySlot();
  }

  //AbilitiesLayout->InvalidateLayoutAndVolatility();
}

void UAbilitiesWidget::FillBarSlots(const TArray<FBar>& Bars)
{
  AWorkshopGameModeBase* GameMode = Cast<AWorkshopGameModeBase>(GetWorld()->GetAuthGameMode());
  if (!GameMode)
  {
    UE_LOG(LogTemp, Error, TEXT("UAbilitiesWidget tried to FillBarSlots but GameMode isn't inherited from WorkshopGameModeBase"));
    return;
  }

  size_t SlotsIndex = 0;
  for (const FBar& Bar : Bars)
  {
    if (SlotsIndex == BarsSlots.Num())
    {
      AddBarSlot();
    }

    BarsSlots[SlotsIndex]->SetBarAndText(Bar, GameMode);

    SlotsIndex++;
  }

  while (BarsSlots.Num() > Bars.Num())
  {
    RemoveBarSlot();
  }

  //BarsLayout->InvalidateLayoutAndVolatility();
}

void UAbilitiesWidget::NativePreConstruct()
{
  if (!AbilitySlotClass)
  {
    UE_LOG(LogTemp, Error, TEXT("AbilitySlotClass must be set in AbilitiesWidget!"));
    Super::NativePreConstruct();
    return;
  }

  bIsFocusable = true;

  HideName();

  Super::NativePreConstruct();
}

void UAbilitiesWidget::AddAbilitySlot()
{
  UAbilitySlot* NewAbilitySlot = WidgetTree->ConstructWidget<UAbilitySlot>(AbilitySlotClass);
  VerticalBoxSlots.Add(NewAbilitySlot);

  UVerticalBoxSlot* NewSlot = AbilitiesLayout->AddChildToVerticalBox(NewAbilitySlot);
  NewSlot->SetSize(FSlateChildSize(ESlateSizeRule::Type::Fill));
}

void UAbilitiesWidget::AddBarSlot()
{
  UMixedProgressBar* NewBarSlot = WidgetTree->ConstructWidget<UMixedProgressBar>(BarSlotClass);
  BarsSlots.Add(NewBarSlot);

  UVerticalBoxSlot* NewSlot = BarsLayout->AddChildToVerticalBox(NewBarSlot);
  NewSlot->SetSize(FSlateChildSize(ESlateSizeRule::Type::Fill));
}

void UAbilitiesWidget::RemoveAbilitySlot()
{
  VerticalBoxSlots.Last()->RemoveFromParent();
  VerticalBoxSlots.Pop();
}

void UAbilitiesWidget::RemoveBarSlot()
{
  BarsSlots.Last()->RemoveFromParent();
  BarsSlots.Pop();
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

void UAbilitiesWidget::SetInteractiveObjectData(const FInteractiveCore& Data)
{
  AWorkshopGameModeBase* GameMode = Cast<AWorkshopGameModeBase>(GetWorld()->GetAuthGameMode());
  if (!GameMode)
  {
    UE_LOG(LogTemp, Error, TEXT("AbilitiesWidget tried to SetObjectName but GameMode isn't inherited from WorkshopGameModeBase"));
    return;
  }

  NameText->SetVisibility(ESlateVisibility::Visible);

  FString CollectedObjectInfo = "";

  for (int32 CT : Data.CTsOfObject)
  {
    FCTData CTData = GameMode->GetCTData(CT);

    CollectedObjectInfo += \
      "<" + CTData.TextStyle.ToString() + ">" + CTData.ShownName.ToString() + "</> ";
  }

  NameText->SetText(FText::FromString(CollectedObjectInfo + Data.InteractiveName.ToString()));
}

void UAbilitiesWidget::HideName()
{
  NameText->SetVisibility(ESlateVisibility::Hidden);
}
