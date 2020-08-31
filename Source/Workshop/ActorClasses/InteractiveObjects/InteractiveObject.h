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
// and exchange information between other Interactive objects
UCLASS(Blueprintable)
class WORKSHOP_API AInteractiveObject : public AActor
{
	GENERATED_BODY()

protected:
  UPROPERTY() int32 InteractiveType = static_cast<int32>(EInteractiveType::Nothing);

  TSet<AInteractiveObject*> DependenciesArray;
  TSet<AInteractiveObject*> InfluencesArray;

  // ------------- //
  // Visualisation //
  // ------------- //

  UPROPERTY() USceneComponent* RootScene;

  UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
  UIconComponent* InteractivityIcon;

  // If object in array is influenced by this object it will be shown unavailable.
  // Otherwise it will be shown available.
  void ShowIconsDependingOnInfluence();

  void HideDisplayedIcons();

  // ------------------ //
  // Object Statisctics //
  // ------------------ //

  // Statisctics in form of strings
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "InteractivitySettings | Statistics")
  TMap<int32, FName> StringStats =
  {
    {ObjectNameStatID, DefaultStringValue},
  };

  // Statisctics in form of integers
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "InteractivitySettings | Statistics")
  TMap<int32, int32> IntegerStats;

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

  // -------------------- //
  // Turn-based mechanics //
  // -------------------- //

  // Array of effects which are applied in the current state.
  UPROPERTY() TArray<UEffectData*> AccumulatedEffects;

  // Called when the game starts or when spawned.
  virtual void BeginPlay() override;

  void RemoveEffectByIndex(int32 EffectIndex);

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

  // ----------------------- //
  // Connection with Manager //
  // ----------------------- //

  UFUNCTION(BlueprintCallable)
  ARegistrationManager* GetManager() const;

  UFUNCTION(BlueprintCallable)
  void ConnectToManager(ARegistrationManager* NewManager);

  UFUNCTION(BlueprintCallable)
  virtual void PickedAsCentral();

  UFUNCTION(BlueprintCallable)
  bool IsCentral();

  UFUNCTION(BlueprintCallable)
  virtual void UnpickedAsCentral();

  UFUNCTION(BlueprintCallable)
  virtual void PickedAsTarget();

  UFUNCTION(BlueprintCallable)
  virtual void UnpickedAsTarget();

  UFUNCTION(BlueprintCallable)
  void Pick();

  // --------------------------- //
  // Influences and dependencies //
  // --------------------------- //

  UFUNCTION(BlueprintCallable)
  void AddInfluenceOn(AInteractiveObject* object);

  UFUNCTION(BlueprintCallable)
  void RemoveDependenceFrom(AInteractiveObject * object);

  UFUNCTION(BlueprintCallable)
  void ClearInflunces();

  UFUNCTION(BlueprintCallable)
  void ClearDependencies();

protected:
  // -------- //
  // CT usage //
  // -------- //

  std::shared_ptr<Node<AInteractiveObject>>& GetNodeForCT();

  const TArray<int32>* GetCTs() const;

public:
  // --------------- //
  // Get information //
  // --------------- //

  UFUNCTION(BlueprintCallable)
  int32 GetInteractiveType() const;

  UFUNCTION(BlueprintCallable)
  FName GetInteractiveObjectName() const;

  UFUNCTION(BlueprintCallable)
  TArray<FString> GetCTsNamesOfObject() const;

  // Happens when player chooses this object.
  UFUNCTION(BlueprintCallable)
  virtual FString GatherInformation() const;

  // Visual interpretation of connections.
  virtual void ShowInfluences() const;

  // ------------------ //
  // Turn-based actions //
  // ------------------ //

  UFUNCTION(BlueprintCallable)
  virtual void SetTurn(ETurnPhase TurnPhase);

  // ------ //
  // Others //
  // ------ //

  friend class AInteractiveObject;
  //???? change to friend void AddInfluenceOn(AInteractiveObject*);
  //???? and friend void RemoveDependenceFrom(AInteractiveObject*);
  friend class CTsGraph<int32, AInteractiveObject>;
  friend class UBuildAbility;

  // Effects are gameplay mechanics so they should be able to modify InteractiveObject
  friend class UEffectData;
  friend class UAdvantageEffectData;
  friend class UChangeStatEffectData;
};
