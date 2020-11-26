//...

#include "Bar.h"
#include "Math/UnrealMathUtility.h"

void FBar::ChangeBarBy(float Value)
{
  FScopeLock Lock(&Change);
  CurrentValue = FMath::Clamp(CurrentValue + Value, 0.f, 1.f);
  
  check(BarLimits.Num());

  size_t LeftLimit = 0, RightLimit = BarLimits.Num() - 1;
  while (LeftLimit < RightLimit - 1)
  {
    size_t MidLimit = LeftLimit + (RightLimit - LeftLimit) / 2;
    if (CurrentValue >= BarLimits[MidLimit])
    {
      LeftLimit = MidLimit;
    }
    else
    {
      RightLimit = MidLimit;
    }
  }

  bIsActive = (LeftLimit % 2) ? true : false;
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
  bIsActive = AnotherBar.bIsActive;
}

FBar& FBar::operator=(const FBar & AnotherBar)
{
  BarLimits = AnotherBar.BarLimits;
  DefaultValue = AnotherBar.DefaultValue;
  StatID = AnotherBar.StatID;
  CurrentValue = AnotherBar.CurrentValue;
  bIsActive = AnotherBar.bIsActive;

  return *this;
}
