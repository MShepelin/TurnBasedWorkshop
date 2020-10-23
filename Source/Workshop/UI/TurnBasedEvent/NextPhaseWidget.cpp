// ...

#include "NextPhaseWidget.h"

void UNextPhaseWidget::PushButton()
{
  if (!PlayerController)
  {
    return;
  }

  if (PlayerController)
  {

  }
}

void UNextPhaseWidget::NativePreConstruct()
{
  Super::NativePreConstruct();

  NextPhase->OnPressed.AddDynamic(this, &UNextPhaseWidget::PushButton);
}
