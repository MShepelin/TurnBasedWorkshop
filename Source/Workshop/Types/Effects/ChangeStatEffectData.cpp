// Fill out your copyright notice in the Description page of Project Settings.

#include "ChangeStatEffectData.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveCharacter.h"
#include "../Nonblueprintable/GameConstants.h"


UChangeStatEffectData::UChangeStatEffectData()
{

}

void UChangeStatEffectData::ResolveOn(AInteractiveObject* TargetObject)
{
  for (FBar& Stat : TargetObject->InteractiveDataCore.Stats)
  {
    if (Stat.StatID == StatID)
    {
      Stat.ChangeBarBy(EffectValue);
      break;
    }
  }
}

FString UChangeStatEffectData::GetInfoString(ARegistrationManager* UsedManager)
{
  FString InfoString = UsedManager->GetStatNameByID(StatID) + " " + ((EffectValue > 0) ? "+" : "") + FString::FromInt(EffectValue);
  InfoString += Super::GetInfoString(UsedManager);

  return InfoString;
}
