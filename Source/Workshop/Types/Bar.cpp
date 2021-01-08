//...

#include "Bar.h"
#include "Nonblueprintable/GameConstants.h"
#include "Math/UnrealMathUtility.h"

bool FBar::ChangeBarBy(int8 Value)
{
  FScopeLock Lock(&Change);

  if (Value > 0)
  {
    (Value >= BAR_MAX_VALUE - CurrentValue) ? CurrentValue = BAR_MAX_VALUE : CurrentValue += Value;
  }
  else
  {
    (CurrentValue + Value < 0) ? CurrentValue = 0 : CurrentValue += Value;
  }
  
  check(BarLimits.Num());
  size_t LeftLimit = 0, RightLimit = BarLimits.Num() - 1;
  while (LeftLimit < RightLimit - 1)
  {
    size_t MidLimit = LeftLimit + (RightLimit - LeftLimit) / 2;
    (CurrentValue >= BarLimits[MidLimit]) ? LeftLimit = MidLimit : RightLimit = MidLimit;
  }

  return (bIsActive = (LeftLimit % 2) ? true : false);
}

void FBar::ResetBar()
{
  CurrentValue = DefaultValue;

  ChangeBarBy(0);
}

bool FBar::IsActive() const
{
  return bIsActive;
}

FBar::FBar()
{
  
}

FBar::FBar(const FBar & AnotherBar)
{
  BarLimits = AnotherBar.BarLimits;
  DefaultValue = AnotherBar.DefaultValue;
  StatID = AnotherBar.StatID;
  CurrentValue = AnotherBar.CurrentValue;
  bIsActive = (AnotherBar.bIsActive) ? true : false;
}

FBar& FBar::operator=(const FBar & AnotherBar)
{
  BarLimits = AnotherBar.BarLimits;
  DefaultValue = AnotherBar.DefaultValue;
  StatID = AnotherBar.StatID;
  CurrentValue = AnotherBar.CurrentValue;
  bIsActive = (AnotherBar.bIsActive) ? true : false;

  return *this;
}
