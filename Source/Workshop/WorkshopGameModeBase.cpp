// Copyright Epic Games, Inc. All Rights Reserved.


#include "WorkshopGameModeBase.h"

FCTData AWorkshopGameModeBase::GetCTData(int32 CT) const
{
  if (CTDataTable == nullptr)
  {
    UE_LOG(LogTemp, Error, TEXT("CT Data Table must be chosen for the GameMode!"));
    return FCTData();
  }

  FName CTAsName = IntToFName(CT);
  FCTData *CTDataPtr = nullptr;
  if (nullptr != (CTDataPtr = CTDataTable->FindRow<FCTData>(CTAsName, TEXT("Get CT Data by ID from GameMode"))))
  {
    return *CTDataPtr;
  }

  return FCTData();
}

TArray<int32> AWorkshopGameModeBase::GetCTIDs() const
{
  if (CTDataTable == nullptr)
  {
    UE_LOG(LogTemp, Error, TEXT("CT Data Table must be chosen for the GameMode!"));
    return {};
  }

  TArray<int32> CTIDs;
  TCHAR CTAsChars[1024];
  for (FName CTAsName : CTDataTable->GetRowNames())
  {
    CTAsName.GetPlainNameString(CTAsChars);
    CTIDs.Add(FCString::Atoi(CTAsChars));
  }

  return CTIDs;
}

FString AWorkshopGameModeBase::GetStatNameByID(int32 StatID) const
{
  if (StatDataTable == nullptr)
  {
    UE_LOG(LogTemp, Error, TEXT("Stat Data Table must be chosen for the GameMode!"));
    return {};
  }

  FName StatIDAsName = IntToFName(StatID);
  FStatData *StatDataPtr = nullptr;
  if (nullptr != (StatDataPtr = StatDataTable->FindRow<FStatData>(StatIDAsName, TEXT("Get Stat Data by ID from GameMode"))))
  {
    return StatDataPtr->ShownName;
  }

  return {};
}

FName AWorkshopGameModeBase::IntToFName(int32 Value)
{
  bool bIsNumberNegative = (Value < 0);
  const int32 TempBufferSize = 16; // Big enough size for a null-terminated string
  ANSICHAR TempNum[TempBufferSize];
  int32 TempIndex = TempBufferSize;

  do
  {
    int32 Digit = Value % 10;
    if (bIsNumberNegative) Digit = -Digit;

    TempNum[--TempIndex] = Digit + '0';
    Value /= 10;
  } while (Value);

  if (bIsNumberNegative) TempNum[--TempIndex] = '-';

  const ANSICHAR* CharPtr = TempNum + TempIndex;
  const int32 NumChars = TempBufferSize - TempIndex;

  return FName(NumChars, CharPtr);
}
