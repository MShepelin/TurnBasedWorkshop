// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedHUD.h"


void ATurnBasedHUD::HideAbilities()
{
  AbilitiesWidget->SetVisibility(ESlateVisibility::Hidden);
}

void ATurnBasedHUD::ShowAbilities()
{
  AbilitiesWidget->SetVisibility(ESlateVisibility::Visible);
}

void ATurnBasedHUD::SetCentralManager(ARegistrationManager* NewManager)
{
  if (CentralManager)
  {
    UE_LOG(LogTemp, Warning, TEXT("For now, CentralManager can't be changed after it's set"));
    return;
  }

  CentralManager = NewManager;
}

UAbilitiesWidget* ATurnBasedHUD::GetAbilitiesWidget()
{
  return AbilitiesWidget;
}

void ATurnBasedHUD::SetAbilitiesWidget(UAbilitiesWidget* NewWidget)
{
  AbilitiesWidget = NewWidget;
}
