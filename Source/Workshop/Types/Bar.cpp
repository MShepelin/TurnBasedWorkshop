//...

#include "Bar.h"
#include "Math/UnrealMathUtility.h"

void FBar::ChangeBarBy(int32 Value)
{
  CurrentValue = FMath::Clamp(CurrentValue + Value, BarLimits[0], BarLimits[BarLimits.Num() - 1]);

  size_t LeftLimit = 0, RightLimit = BarLimits.Num();
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
