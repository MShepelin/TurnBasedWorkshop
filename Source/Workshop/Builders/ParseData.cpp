// Fill out your copyright notice in the Description page of Project Settings.

#include "ParseData.h"
#include "Workshop/ActorClasses/Managers/RegistrationManager.h"

/*
FString UParseData::CollectAccumulatedEffects(AInteractiveObject* TargetObject)
{
  ARegistrationManager* Manager = TargetObject->GetManager();

  FString ParsedString = "";
  for (TTuple<int32, FBar> IntegerStat : TargetObject->InteractiveDataCore.IntegerStats)
  {
    ParsedString += Manager->GetStatNameByID(IntegerStat.Get<0>()) + " " + FString::FromInt(IntegerStat.Get<1>().CurrentValue) + "\n";
  }

  return ParsedString;
}
*/
