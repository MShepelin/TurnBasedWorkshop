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

// Interactive object is paired with Manager to support turn-based actions 
// and exchange information between other Interactive objects.
UCLASS(Abstract, Blueprintable)
class WORKSHOP_API AInteractiveObject : public AActor
{
	GENERATED_BODY()

protected:
  // Class description:
  // Has 2 states: awake and asleep, which corresponds to visibility of Icon component.
  // One InteractiveObject can "influence" another one, which can be used for gameplay.
  // Manager can check if object is influenced by another one, initiate influence or remove it.

  // ---------------------- //
  // Interactive Properties //
  // ---------------------- //

  // InteractiveObjects linked to this object to influnece it
  TSet<AInteractiveObject*> DependsOn;
  // InteractiveObjects linked to this object to depend on it
  TSet<AInteractiveObject*> InfluencesOn;
  
  // ------------- //
  // Visualisation //
  // ------------- //

  // Shows if object is awake and if it used by player.
  UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
  UIconComponent* InteractivityIcon;
  // Used to receive player's input.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
  UBoxComponent* CollisionBox;

  // ------------------ //
  // CTs System Support //
  // ------------------ //

  // Manager which currently handles this object.
  UPROPERTY() ARegistrationManager* MainManager = nullptr;
  // Node for CT system of MainManager.
  std::shared_ptr<Node<AInteractiveObject>> NodeForCT = nullptr;

public:
  // ----------------- //
  // Object Statistics //
  // ----------------- //

  // Represents gameplay variables abstracted from other objects, 
  // which can be moved from object to object
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
  FInteractiveCore InteractiveDataCore;

private:
  // -------- //
  // CT usage //
  // -------- //

  // Used for CT system
  std::shared_ptr<Node<AInteractiveObject>>& GetNodeForCT();
  // Used for CT system
  const TArray<int32>* GetCTs() const;

protected:
  // Called when the game starts or when spawned.
  virtual void BeginPlay() override;

public:
  // Contructor.
  AInteractiveObject();

  // ------------------------- //
  // AActor functions overload //
  // ------------------------- //

  // Called before construction script.
  virtual void OnConstruction(const FTransform & Transform) override;

  // Allow actors to initialize themselves on the C++ side after all of their components have been initialized, only called during gameplay
  virtual void PostInitializeComponents() override;

  // Called every frame.
	virtual void Tick(float DeltaTime) override;

  // Called whenever this actor is being removed from a level
  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

  // ------------------ //
  // Manager Connection //
  // ------------------ //

  // Get Manager which currently handles this object.
  UFUNCTION(BlueprintCallable)
  ARegistrationManager* GetManager() const;

  // Check if MainManager considers this object as central.
  UFUNCTION(BlueprintCallable)
  bool IsCentralInManager() const;

  // Called when player wants to pick this object.
  UFUNCTION(BlueprintCallable) virtual void Pick();

  // Called when the player wants to pick this object as central (both for view and MainManager).
  UFUNCTION() virtual void PickedAsCentral();
  // Called when the player wants to stop using object as central (both for view and MainManager).
  UFUNCTION() virtual void UnpickedAsCentral();
  // Called when the player wants to pick this object as target of other object's influence.
  UFUNCTION() virtual void PickedAsTarget();
  // Called when the player wants to stop using object as target of other object's influence.
  UFUNCTION() virtual void UnpickedAsTarget();

  // --------------------------- //
  // Influences and dependencies //
  // --------------------------- //

  // Called to start influence on other object.
  UFUNCTION() virtual void AddInfluenceOn(AInteractiveObject* object);
  // Called to stop influence on this object by another one.
  UFUNCTION() virtual void RemoveDependenceFrom(AInteractiveObject * object);
  // Called to remove influence of this obejct.
  UFUNCTION() virtual void ClearInflunces();
  // Called to stop any influence on this obejct.
  UFUNCTION() virtual void ClearDependencies();

  // --------------- //
  // Get information //
  // --------------- //

#if WITH_EDITOR
  // Visual interpretation of connections.
  UFUNCTION(BlueprintCallable) 
  virtual void ShowInfluences() const;
#endif

  // Get mask which gives information about used class and control properties.
  UFUNCTION(BlueprintCallable)
  int32 GetInteractiveType() const;

  // ------ //
  // Others //
  // ------ //

  UFUNCTION(BlueprintCallable)
  virtual void PlayAnimation(int32 AnimationId, bool bWaitUntilEnds);

  UFUNCTION(BlueprintCallable)
  virtual void UpdateCharacterStatus(); //++++ RENAME

  // Called to refresh appearance.
  UFUNCTION() virtual void RefreshInteractive();

  // Resolve effects depeding on their resolve phase.
  // UFUNCTION() void ResolveAccumulatedEffects(ETurnPhase TurnPhase);

  // Interactive objects often exchange information, 
  // so friend status is used for optimisation and code clearness.
  friend class AInteractiveObject;

  // Agreement: ARegistrationManager is the only class, 
  // which can change MainManager.
  friend class ARegistrationManager;

  // CTsGraph is build to be fast tool to search large number of Interactive objects,
  // so this is used to optimise access to needed information
  friend class CTsGraph<int32, AInteractiveObject>;
  //++++ remove functions as CTsGraph has access to protected members

  //???? make class which can process gathering information about this class
  //     to present it for hud
};
