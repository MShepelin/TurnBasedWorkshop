// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Workshop/Types/CTData.h"
#include "Workshop/Types/StatData.h"
#include "WorkshopGameModeBase.generated.h"

/** Basic mode base for a turn-based game. It stores global data about names of stats and CTs. */
UCLASS()
class WORKSHOP_API AWorkshopGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  UDataTable* CTDataTable;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  UDataTable* StatDataTable;

public:
  static FName IntToFName(int32 Value);

  UFUNCTION(BlueprintCallable)
  FCTData GetCTData(int32 CT) const;

  TArray<int32> GetCTIDs() const;

  FString GetStatNameByID(int32 StatID) const;
};
