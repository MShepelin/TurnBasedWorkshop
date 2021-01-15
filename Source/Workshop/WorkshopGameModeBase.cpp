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
  // Convert int32 to FName assuming base ten using the code from FString::AppendInt.
  const ANSICHAR* DigitToChar = "9876543210123456789";
  constexpr int32 ZeroDigitIndex = 9;
  bool bIsNumberNegative = (Value < 0);
  const int32 TempBufferSize = 16; // 16 is big enough
  ANSICHAR TempNum[TempBufferSize];
  int32 TempAt = TempBufferSize;

  do
  {
    TempNum[--TempAt] = DigitToChar[ZeroDigitIndex + (Value % 10)];
    Value /= 10;
  } while (Value);

  if (bIsNumberNegative)
  {
    TempNum[--TempAt] = '-';
  }

  const ANSICHAR* CharPtr = TempNum + TempAt;
  const int32 NumChars = TempBufferSize - TempAt;

  return FName(NumChars, CharPtr);
}
