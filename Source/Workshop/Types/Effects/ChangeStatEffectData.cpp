// Fill out your copyright notice in the Description page of Project Settings.

#include "ChangeStatEffectData.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveCharacter.h"
#include "../Nonblueprintable/GameConstants.h"


UChangeStatEffectData::UChangeStatEffectData()
{

}

void UChangeStatEffectData::ResolveOn(AInteractiveObject* TargetObject)
{
  if (StatID >= 0 && StatID < CharacterIntegerStats)
  {
    AInteractiveCharacter* TargetCharacter = Cast<AInteractiveCharacter>(TargetObject);
    check(TargetCharacter);

    check(TargetCharacter->CharacterDataCore.CharacterStats.Num() > StatID);
    TargetCharacter->CharacterDataCore.CharacterStats[StatID].ChangeBarBy(EffectValue);
    return;
  }

  if (TargetObject->InteractiveDataCore.IntegerStats.Find(StatID))
  {
    TargetObject->InteractiveDataCore.IntegerStats[StatID].ChangeBarBy(EffectValue);
  }
  //else if (bIsForciblyAdded)
  //{
  //  TargetObject->InteractiveDataCore.IntegerStats.Add(StatID, EffectValue);
  //}
}

FString UChangeStatEffectData::GetInfoString()
{
  FString InfoString = std::move(Super::GetInfoString());

  //InfoString += Manager->GetStatNameByID(StatID) + " " + FString::FromInt(EffectValue);

  return InfoString;
}
