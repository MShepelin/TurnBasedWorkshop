#include "MixedProgressBar.h"


void UMixedProgressBar::SetBar(const FBar& Bar)
{
  BarToDraw = Bar;
}

void UMixedProgressBar::SetBarAndText(const FBar& Bar, ARegistrationManager *Manager)
{
  BarToDraw = Bar;
  BackgroundText = FText::FromString(Manager->GetStatNameByID(BarToDraw.StatID));
}

