// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "Components/BillboardComponent.h"
#include "Workshop/Types/Nonblueprintable/CTsSystem.h"
#include "Workshop/Types/Nonblueprintable/GameConstants.h"
#include "RegistrationManager.generated.h"

class AInteractiveObject;

// Manager connects Interactive objects, provides search by CTs, serves as setup for gameplay events.
UCLASS()
class WORKSHOP_API ARegistrationManager : public AActor
{
	GENERATED_BODY()

protected:
  // Icon to visualise Manager's position in the world.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "ManagerVisuals")
  UBillboardComponent* ManagerIcon;

  // Here are all CTs available in this manager.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "ManagerSettings", meta = (ClampMin = "1"));
  TMap<int32, FString> CTsToNameMap;

  TArray<int32> NecessaryCTs = 
  { 
    PlayerControlledCharacterCT, 
    CharacterOutOfControlCT,
  };

  // Here are all stats available in this manager.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "ManagerSettings")
  TMap<int32, FString> StatIDToNameMap =
  {
    {ObjectNameStatID, "Name"}
  };

  // Used for search by CTs.
  CTsGraph<int32, AInteractiveObject> CTsSystem;

  // This object is in current focus of manager, all found connections will be sent to it.
  UPROPERTY() AInteractiveObject* CentralObject = nullptr;

	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
  // Sets default values for this actor's properties.
  ARegistrationManager();

  // ------------------------- //
  // AActor functions overload //
  // ------------------------- //

	// Called every frame.
	virtual void Tick(float DeltaTime) override;

  void PostInitProperties() override;

  //---------- //
  // CTs usage //
  //---------- //

  UFUNCTION(BlueprintCallable)
  TArray<AInteractiveObject*> FindObjectsByCTs(const TArray<int32> CTsArray, int32 EnoughNumberOfCTs) const;

  // ----------------------------- //
  // Access to Manager information //
  // ----------------------------- //

  UFUNCTION(BlueprintCallable)
  TArray<AInteractiveObject*> GetAllConnectedObjects() const;

  UFUNCTION(BlueprintCallable)
  FString GetCTName(int32 CTIdentifier) const;

  UFUNCTION(BlueprintCallable)
  FString GetStatNameByID(int32 StatIdentifier) const;

  UFUNCTION(BlueprintCallable)
  bool HasCentralObject() const;

  friend class AInteractiveObject;
};
