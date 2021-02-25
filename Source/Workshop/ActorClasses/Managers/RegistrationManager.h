// Copyright © Dmitriy Shepelin 2021. MIT License.

#pragma once

#include "CoreMinimal.h"
#include <memory>
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "Containers/Queue.h"
#include "Components/BillboardComponent.h"
#include "Workshop/Types/Nonblueprintable/CTsSystem.h"
#include "Workshop/Types/Nonblueprintable/GameConstants.h"
#include "RegistrationManager.generated.h"

class AInteractiveObject;
class AInteractiveAbility;

/**
 * Provides search by CTs for connected Interactive objects, stores awaken Interactive objects.
 */
UCLASS()
class WORKSHOP_API ARegistrationManager : public AActor
{
	GENERATED_BODY()

protected:
  /** Icon to visualise Manager's position in the world. */
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "ManagerVisuals")
  UBillboardComponent* ManagerIcon;

  /** 
   * Used to store managed InteractiveObjects.
   * @note changed only by Awake/PutToSleep functions of InteractiveObjects.
   */
  TArray<AInteractiveObject*, TInlineAllocator<AVERAGE_MANAGED_OBJECTS>> AwakenObjects;

  /** Used for search by CTs. */
  std::unique_ptr<CTsSearch<int32, AInteractiveObject>> CTsSystem;

  /** This object is in current focus of manager, all found connections will be sent to it. */
  UPROPERTY() AInteractiveObject* CentralObject = nullptr;

	virtual void BeginPlay() override;

  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
  ARegistrationManager();

  // ------------------------- //
  // AActor functions overload //
  // ------------------------- //

	virtual void Tick(float DeltaTime) override;

  //---------- //
  // CTs usage //
  //---------- //

  /** Finds all suitable InteractiveObjects. */
  UFUNCTION(BlueprintCallable)
  TArray<AInteractiveObject*> FindObjectsByCTs(
    const TArray<int32>& CTsArray, int32 EnoughNumberOfCTs);

  /** Finds all suitable InteractiveObjects excluding Manager's central object. */
  UFUNCTION(BlueprintCallable)
  TArray<AInteractiveObject*> FindObjectsByCTsWithMask(
    const TArray<int32>& CTsArray, int32 EnoughNumberOfCTs, int32 TargetTypeMask);

  // ---------- //
  // Connection //
  // ---------- //

  /** Connect an object to this manager. */
  UFUNCTION(BlueprintCallable) 
  void ConnectObject(AInteractiveObject* Object);

  /** Disconnect an object from this manager. */
  UFUNCTION(BlueprintCallable)
  void DisconnectObject(AInteractiveObject* Object);

  /** Changes status of InteractiveObjects which are awaken to asleep. */
  UFUNCTION() void PutToSleepManagedObjects(ARegistrationManager* Manager);

  /**
   * If object in array is influenced by central object it will be shown unavailable.
   * Otherwise it will be shown available.
   */
  UFUNCTION() void AwakeByCenterObject(TArray<AInteractiveObject*>& Objects);

  // ----------------------------- //
  // Access to Manager information //
  // ----------------------------- //

  /** Get an array of all objects, which were connected to this manager. */
  UFUNCTION(BlueprintCallable)
  TArray<AInteractiveObject*> GetAllConnectedObjects() const;

  /** Check if this object has a central object. */
  UFUNCTION(BlueprintCallable)
  bool HasCentralObject() const;

  /** 
   * Get an objects, which is considered central by this manager. 
   * Can return nullptr. 
   */
  UFUNCTION(BlueprintCallable)
  AInteractiveObject* GetCentralObject();

  // Agreement: AInteractiveObject is the only class, 
  // which can change Manager's CentralObject and CTsSystem
  friend class AInteractiveObject;

  // Managers are allowed to take objects from each other.
  friend class ARegistrationManager;
};
