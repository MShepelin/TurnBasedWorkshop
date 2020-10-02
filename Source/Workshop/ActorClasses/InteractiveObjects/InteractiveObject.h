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
  // Has 2 states: awake and asleep, which corresponds to visibility of Icon component.

  // ---------------------- //
  // Interactive Properties //
  // ---------------------- //

  TSet<AInteractiveObject*> DependenciesSet;
  TSet<AInteractiveObject*> InfluencesSet;

  // ------------- //
  // Visualisation //
  // ------------- //

  UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
  UIconComponent* InteractivityIcon;

  // ------------------ //
  // CTs System Support //
  // ------------------ //

  // Manager is a way for objects to find other Interactive objects to influence.
  UPROPERTY() ARegistrationManager* MainManager = nullptr;

  // Node for CT system.
  // Any object can be added only to one system of CTs
  std::shared_ptr<Node<AInteractiveObject>> NodeForCT = nullptr;

  // Called when the game starts or when spawned.
  virtual void BeginPlay() override;

  inline void RemoveEffectByIndex(int32 EffectIndex);

public:
  // ----------------- //
  // Object Statistics //
  // ----------------- //

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
  FInteractiveCore InteractiveDataCore;

private:
  // -------- //
  // CT usage //
  // -------- //

  std::shared_ptr<Node<AInteractiveObject>>& GetNodeForCT();

  const TArray<int32>* GetCTs() const;

public:
  AInteractiveObject();

  // ------------------------- //
  // AActor functions overload //
  // ------------------------- //

  // Called before construction script.
  virtual void OnConstruction(const FTransform & Transform) override;

  virtual void PostInitProperties() override;

  // Called every frame.
	virtual void Tick(float DeltaTime) override;

  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

  // ------------------ //
  // Manager Connection //
  // ------------------ //

  UFUNCTION(BlueprintCallable)
  ARegistrationManager* GetManager() const;

  UFUNCTION(BlueprintCallable)
  bool IsCentralInManager() const;

  UFUNCTION() virtual void PickedAsCentral();
  UFUNCTION() virtual void UnpickedAsCentral();
  UFUNCTION() virtual void PickedAsTarget();
  UFUNCTION() virtual void UnpickedAsTarget();

  // Picked by controller with purpose to choose targets to influence on.
  UFUNCTION(BlueprintCallable)
  void Pick();

  // --------------------------- //
  // Influences and dependencies //
  // --------------------------- //

  UFUNCTION() void AddInfluenceOn(AInteractiveObject* object);
  UFUNCTION() void RemoveDependenceFrom(AInteractiveObject * object);
  UFUNCTION() void ClearInflunces();
  UFUNCTION() void ClearDependencies();

  // Attention: these 4 functions above don't change appearence of connections.

  // --------------- //
  // Get information //
  // --------------- //

#if WITH_EDITOR
  // Visual interpretation of connections.
  UFUNCTION(BlueprintCallable) 
  virtual void ShowInfluences() const;
#endif

  UFUNCTION(BlueprintCallable)
  int32 GetInteractiveType() const;

  // ------ //
  // Others //
  // ------ //

  // Special function which should be called after any properties are changed in any unusual way.
  UFUNCTION() virtual void RefreshInteractive();

  // Resolve effects depeding on their resolve phase.
  UFUNCTION() void ResolveAccumulatedEffects(ETurnPhase TurnPhase);

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
