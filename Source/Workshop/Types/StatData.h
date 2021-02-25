// Copyright © Dmitriy Shepelin 2021. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "StatData.generated.h"

USTRUCT(BlueprintType)
struct WORKSHOP_API FStatData : public FTableRowBase
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FString ShownName;
};
