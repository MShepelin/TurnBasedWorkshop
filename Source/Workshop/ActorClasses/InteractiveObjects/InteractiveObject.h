// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "Workshop/Types/Nonblueprintable/GameConstants.h"
#include "Workshop/Types/Components/IconComponent.h"
#include "Workshop/Types/Effects/EffectData.h"
#include "Workshop/Types/InteractiveType.h"
#include "Workshop/Types/TurnPhase.h"
#include "Components/BoxComponent.h"
#include "Workshop/Types/InteractiveObjectData/InteractiveCore.h"
#include "InteractiveObject.generated.h"

class UBuildAbility;
class UChangeStatEffectData;
class UAdvantageEffectData;
class AInteractiveObject;

/**
 * Interactive object is a base class which supports:
 * - Stats, which are gameplay values opened to players;
 * - Custom Tags (CTs), which are used to search objects;
 * - Pairing with Registration Manager to use a search by CTs;
 * - Basic interactive behavoir (only when connected to a Registration Manager): 
 *   it can be picked/unpicked as a central object and picked/unpicked as a target for a central object;
 * - Two states called "awake" and "asleep" which are used to show if the object is an available target for a central object
 *   (used to change the visual icon and some cases of picking behavoir);
 * - Connection to other Interactive Objets in terms of influence, which is usually used to save targets for gameplay mechanics
 *   (for example, an object A influences an object B, so B is dependent on the object A);
 * - The exhaust state when this objects is non-interactive (in terms desribed above).
 *
 * @note If an object A is dependent on an object B, it can still influence B
 * @note The word "central" refers to the fact that Registration Managers can have one object 
 *       selected to choose targets for this object's gameplay mechanics
 */
UCLASS(Abstract, Blueprintable)
class WORKSHOP_API AInteractiveObject : public AActor, public CTSearchInterface<int32, AInteractiveObject>
{
	GENERATED_BODY()

protected:
  // -------------------- //
  // Influence Properties //
  // -------------------- //

  /** InteractiveObjects linked to this object to influnece it. */
  TSet<AInteractiveObject*> DependsOn;

  /** InteractiveObjects linked to this object to depend on it. */
  TSet<AInteractiveObject*> InfluencesOn;
  
  // ------------- //
  // Visualisation //
  // ------------- //

  /** Shows if the object is awake (can be chose by the the object is awake and if it is used. */
  UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
  UIconComponent* InteractivityIcon;

  /** Used to receive player's input. */
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
  UBoxComponent* CollisionBox;

  /** Manager which currently handles this object. */
  UPROPERTY() ARegistrationManager* MainManager = nullptr;

protected:
  // ------------------ //
  // CTs System Support //
  // ------------------ //

  /** Node for the graph inside CTsSearch */
  std::shared_ptr<Node<AInteractiveObject>> NodeForCT = nullptr;

  /** Used to set a node for the graph inside CTsSearch */
  virtual std::shared_ptr<Node<AInteractiveObject>>& GetNodeForCT() override;

public:
  /** Used to get all CTs from the object */
  virtual const TArray<int32>* GetCTs() const override;

  // --------------- //
  // Object Settings //
  // --------------- //

  /** The object's settings, which can be moved to another object */
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
  FInteractiveCore InteractiveDataCore;

protected:
  /**
   * Function for an additional construction step.
   */
  UFUNCTION() virtual void BuildInteractive();

  /** Called when the game starts or when spawned. */
  virtual void BeginPlay() override;

public:
  /** Default Constructor. */
  AInteractiveObject();

  // ------------------------- //
  // AActor functions overload //
  // ------------------------- //

  /** Called before construction script. */
  virtual void OnConstruction(const FTransform & Transform) override;

  /** Allow actors to initialize themselves on the C++ side after all of their components have been initialized, only called during gameplay */
  virtual void PostInitializeComponents() override;

  /** Called every frame. */
	virtual void Tick(float DeltaTime) override;

  /** Called whenever this actor is being removed from a level. */
  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

  // ------------------------------------ //
  // Connection with Registration Manager //
  // ------------------------------------ //

  /** Get manager which currently handles this object. */
  UFUNCTION(BlueprintCallable)
  ARegistrationManager* GetManager() const;

  /** Check if this object is considered central for the connected Manager */
  UFUNCTION(BlueprintCallable)
  bool IsCentralInManager() const;

  // -------------------- //
  // Interactive Behavoir //
  // -------------------- //

  /** Called when a player wants to pick this object. */
  UFUNCTION(BlueprintCallable) virtual void Pick();

  /** Called when a player wants to pick this object as central. */
  UFUNCTION() virtual void PickedAsCentral();

  /** Called when a player wants to stop using object as central. */
  UFUNCTION() virtual void UnpickedAsCentral();

  /** Called when a player wants to pick this object as target of other object's influence. */
  UFUNCTION() virtual void PickedAsTarget();

  /** Called when a player wants to stop using object as target of other object's influence. */
  UFUNCTION() virtual void UnpickedAsTarget();

  // --------------------------- //
  // Influences and dependencies //
  // --------------------------- //

  /**
   * Called to start an influence on the other object.
   * Returns true if influence was added successfully and false otherwise.
   */
  UFUNCTION() virtual bool AddInfluenceOn(AInteractiveObject* Object);

  /**
   * Called to remove an influence on the other object.
   * Returns true if dependence was removed successfully and false otherwise.
   */
  UFUNCTION() virtual bool RemoveInfluenceOn(AInteractiveObject * Object);

  /** Called to remove the influence of this object. */
  UFUNCTION() virtual void ClearInflunces();

  /** Called to stop any influence on this object. */
  UFUNCTION() virtual void ClearDependencies();

  // ------ //
  // Others //
  // ------ //

#if WITH_EDITOR
  /** Visual representation of connections. */
  UFUNCTION(BlueprintCallable) 
  virtual void ShowInfluences() const;
#endif

  /** Get a mask with information about types of this Interactive Object (such as control properties). */
  UFUNCTION(BlueprintCallable)
  int32 GetInteractiveType() const;

  /** 
   * Thread-safe way to play animations. 
   * If bWaitUntilEnds is true, the thread will sleep for the duration of the animation.
   */
  UFUNCTION(BlueprintCallable)
  virtual void PlayAnimation(int32 AnimationId, bool bWaitUntilEnds);

  /**
   * If this object is interactive, check if it can still be considered as interactive and
   * make the needed actions if it can't (such as marking as non-interactive). This functions is designed to be thread-safe.
   */
  UFUNCTION(BlueprintCallable)
  virtual void UpdateExhaust();

  /**
   * Called when the objects is exhausted. Unlike UpdateExhaust, this functions doesn't need to be thread-safe.
   */
  UFUNCTION(BlueprintNativeEvent)
  void OnExhausted();
  virtual void OnExhausted_Implementation();

  /**
   * Interactive objects often exchange information, 
   * so friend status is used for optimisation and code clearness.
   */
  friend class AInteractiveObject;

  /** Registration Managers are designed to set private members of Interactive objects. */
  friend class ARegistrationManager;
};
