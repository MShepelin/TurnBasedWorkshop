// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "Workshop/Types/Nonblueprintable/GameConstants.h"
#include "Workshop/Types/Nonblueprintable/CTsSystem.h"
#include "Workshop/Types/Components/IconComponent.h"
#include "Workshop/Types/InteractiveType.h"
#include "Workshop/Types/TurnPhase.h"
#include "InteractiveObject.generated.h"


class UBuildAbility;
class UEffectData;
class ARegistrationManager;


// Interactive object supports CTs classification and dependecy from other Interactive objects.
UCLASS(Blueprintable)
class WORKSHOP_API AInteractiveObject : public AActor
{
	GENERATED_BODY()

protected:
  EInteractiveType InteractiveType = EInteractiveType::Nothing;

  TSet<AInteractiveObject*> DependenciesArray;
  TSet<AInteractiveObject*> InfluencesArray;

  // ------------- //
  // Visualisation //
  // ------------- //

  UPROPERTY() USceneComponent* RootScene;

  UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
  UIconComponent* InteractivityIcon;

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

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "InteractivitySettings | CTs")
  TArray<int32> CTsOfObject;

  // Array of effects which are applied in the current state.
  UPROPERTY() TArray<UEffectData*> AccumulatedEffects;

  // Used to search fro advantages in all effects.
  UPROPERTY() TSet<UEffectData*> SetOfAdvantages;

protected:
  // Called when the game starts or when spawned.
  virtual void BeginPlay() override;

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

  void SetManager(ARegistrationManager* NewManager);

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
  EInteractiveType GetInteractiveType();

  // ------------------ //
  // Turn-based actions //
  // ------------------ //

  UFUNCTION(BlueprintCallable)
  virtual void SetTurn(ETurnPhase TurnPhase);

  // ------ //
  // Others //
  // ------ //

  // Happens when player chooses this object.
  UFUNCTION(BlueprintCallable)
  virtual FString GatherInformation() const;

  // Visual interpretation of connections.
  virtual void ShowInfluences() const;

  UFUNCTION(BlueprintCallable)
  FName GetInteractiveObjectName() const;

  friend void AddInfluenceOn(AInteractiveObject*);
  friend void RemoveInfluenceFrom(AInteractiveObject*);
  friend class CTsGraph<int32, AInteractiveObject>; // for optimisation purposes
  friend class UBuildAbility;                       // for optimisation purposes
  friend class ARegistrationManager;                // to prevent "undefined type" error
};
