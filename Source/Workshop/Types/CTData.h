// Copyright © Dmitriy Shepelin 2021. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CTData.generated.h"

USTRUCT(BlueprintType)
struct WORKSHOP_API FCTData : public FTableRowBase
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FName ShownName;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FName TextStyle;
};
