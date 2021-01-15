#include "MixedProgressBar.h"

void UMixedProgressBar::SetBar(const FBar& Bar)
{
  BarToDraw = Bar;
}

void UMixedProgressBar::SetBarAndText(const FBar& Bar, AWorkshopGameModeBase *GameMode)
{
  BarToDraw = Bar;
  BackgroundText = FText::FromString(GameMode->GetStatNameByID(BarToDraw.StatID));
}
