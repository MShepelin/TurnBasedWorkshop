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
  FScopeLock Lock(&Change);
  return Stats;
}

FInteractiveCore::FInteractiveCore()
{
  InteractiveType = static_cast<int32>(EInteractiveType::Nothing);
}
