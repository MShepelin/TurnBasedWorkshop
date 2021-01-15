// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Workshop/Types/CTdata.h"
#include "WorkshopGameModeBase.generated.h"

UCLASS()
class WORKSHOP_API AWorkshopGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  UDataTable* CTDataTable;

public:
  UFUNCTION(BlueprintCallable)
  FCTData GetCTData(int32 CT) const;

  TArray<int32> GetCTIDs() const;
};
