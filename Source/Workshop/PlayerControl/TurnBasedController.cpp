// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedController.h"

ATurnBasedController::ATurnBasedController()
{
}

void ATurnBasedController::TryToInteract()
{
  return;
}

void ATurnBasedController::SetupInputComponent()
{
  Super::SetupInputComponent();

  InputComponent->BindAction("Interact", IE_Pressed, this, &ATurnBasedController::TryToInteract);

}
