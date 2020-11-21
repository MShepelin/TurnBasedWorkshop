//...

#include "Bar.h"
#include "Math/UnrealMathUtility.h"

void FBar::ChangeBarBy(float Value)
{
  CurrentValue = FMath::Clamp(CurrentValue + Value, 0.f, 1.f);

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

bool FBar::IsActive()
{
  return bIsActive;
}
