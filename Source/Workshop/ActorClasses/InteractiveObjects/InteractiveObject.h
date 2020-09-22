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
#include "InteractiveObject.generated.h"


class UBuildAbility;
class UChangeStatEffectData;
class UAdvantageEffectData;


// Interactive object are paired with Managers to support turn-based actions 
// and exchange information between other Interactive objects.
// Has conditions of being awake or asleep, and being available or unavailable.
UCLASS(Abstract, Blueprintable)
class WORKSHOP_API AInteractiveObject : public AActor
{
	GENERATED_BODY()

protected:
  UPROPERTY() int32 InteractiveType = static_cast<int32>(EInteractiveType::Nothing);

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
  // (For multiple CT systems object-wrapper should be used).
  std::shared_ptr<Node<AInteractiveObject>> NodeForCT = nullptr;

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "InteractivitySettings | CTs", meta = (ClampMin = "1"));
  TArray<int32> CTsOfObject;

  // Called when the game starts or when spawned.
  virtual void BeginPlay() override;

  inline void RemoveEffectByIndex(int32 EffectIndex);

public:
  // ----------------- //
  // Object Statistics //
  // ----------------- //

  // Statisctics in form of strings
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "InteractivitySettings | Statistics")
    TMap<int32, FName> StringStats =
  {
    {ObjectNameStatID, DefaultStringValue},
  };

  // Statisctics in form of integers
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "InteractivitySettings | Statistics")
    TMap<int32, int32> IntegerStats;

  // Array of effects which are applied in the current state.
  UPROPERTY() TArray<UEffectData*> AccumulatedEffects;

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
  bool IsCentral() const;

  UFUNCTION() virtual void PickedAsCentral();
  UFUNCTION() virtual void UnpickedAsCentral();
  UFUNCTION() virtual void PickedAsTarget();
  UFUNCTION() virtual void UnpickedAsTarget();

  // Picked by controller with aim to choose targets to influence on.
  UFUNCTION(BlueprintCallable)
  void Pick();

  // --------------------------- //
  // Influences and dependencies //
  // --------------------------- //

  UFUNCTION() void AddInfluenceOn(AInteractiveObject* object);
  UFUNCTION() void RemoveDependenceFrom(AInteractiveObject * object);
  UFUNCTION() void ClearInflunces();
  UFUNCTION() void ClearDependencies();

  // These 4 functions above don't change appearence of connections.

  // --------------- //
  // Get information //
  // --------------- //

  UFUNCTION(BlueprintCallable)
  int32 GetInteractiveType() const;

  //UFUNCTION(BlueprintCallable)
  //FName GetInteractiveObjectName() const;

  //UFUNCTION(BlueprintCallable)
  //TArray<FString> GetCTsNamesOfObject() const;

  // REMAKE
  // Happens when player chooses this object.
  // UFUNCTION(BlueprintCallable)
  // virtual FString GatherInformation() const;

  // ------ //
  // Others //
  // ------ //

  // Resolve effects depeding on their resolve phase.
  UFUNCTION() void ResolveAccumulatedEffects(ETurnPhase TurnPhase);

#if WITH_EDITOR
  // Visual interpretation of connections.
  UFUNCTION() virtual void ShowInfluences() const;
#endif

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

  // Effects and functions from UBuildAbility create gameplay mechanics 
  // so they should be able to modify InteractiveObject
  //friend class UBuildAbility;
  //friend class UEffectData;

  //???? make class which can process gathering information about this class
  //     to present it for hud
};
