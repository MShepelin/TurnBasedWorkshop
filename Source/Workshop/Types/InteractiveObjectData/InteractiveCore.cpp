//...

#include "InteractiveCore.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveObject.h"
#include "Workshop/Types/InteractiveType.h"

FInteractiveCore::FInteractiveCore(const FInteractiveCore & AnotherCore)
{
  InteractiveType = AnotherCore.InteractiveType;
  CTsOfObject = AnotherCore.CTsOfObject;
  InteractiveName = AnotherCore.InteractiveName;
  Stats = AnotherCore.Stats;
}

FInteractiveCore& FInteractiveCore::operator=(const FInteractiveCore & AnotherCore)
{
  InteractiveType = AnotherCore.InteractiveType;
  CTsOfObject = AnotherCore.CTsOfObject;
  InteractiveName = AnotherCore.InteractiveName;
  Stats = AnotherCore.Stats;

  return *this;
}

TArray<FBar> FInteractiveCore::GetStats()
{
  FScopeLock Lock(&GettingStats);
  return Stats;
}

FInteractiveCore::FInteractiveCore()
{
  InteractiveType = static_cast<uint8>(EInteractiveType::Nothing);
}
