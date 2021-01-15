// Copyright Epic Games, Inc. All Rights Reserved.


#include "WorkshopGameModeBase.h"

FCTData AWorkshopGameModeBase::GetCTData(int32 CT)
{
  if (CTDataTable == nullptr)
  {
    UE_LOG(LogTemp, Error, TEXT("CT Data Table must be chosen for the GameMode!"));
    return FCTData();
  }

  // Convert int32 to FName assuming base ten using the code from FString::AppendInt.
  const ANSICHAR* DigitToChar = "9876543210123456789";
  constexpr int32 ZeroDigitIndex = 9;
  bool bIsNumberNegative = (CT < 0);
  const int32 TempBufferSize = 16; // 16 is big enough
  ANSICHAR TempNum[TempBufferSize];
  int32 TempAt = TempBufferSize;

  do
  {
    TempNum[--TempAt] = DigitToChar[ZeroDigitIndex + (CT % 10)];
    CT /= 10;
  } while (CT);

  if (bIsNumberNegative)
  {
    TempNum[--TempAt] = '-';
  }

  const ANSICHAR* CharPtr = TempNum + TempAt;
  const int32 NumChars = TempBufferSize - TempAt;

  FName CTAsName = FName(NumChars, CharPtr);
  FCTData *CTDataPtr = nullptr;
  if (nullptr != (CTDataPtr = CTDataTable->FindRow<FCTData>(CTAsName, TEXT("Get CT Data from GameMode"))))
  {
    return *CTDataPtr;
  }

  return FCTData();
}
