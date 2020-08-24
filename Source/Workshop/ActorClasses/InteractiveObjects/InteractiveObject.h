// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "Workshop/Types/Nonblueprintable/GameConstants.h"
#include "Workshop/Types/Nonblueprintable/CTsSystem.h"
#include "Workshop/Types/Components/IconComponent.h"
#include "Workshop/Types/InteractiveType.h"
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
  EInteractiveType InteractiveType = EInteractiveType::Base;

  TSet<AInteractiveObject*> DependenciesArray;
  TSet<AInteractiveObject*> InfluencesArray;

  // ------------- //
  // Visualisation //
  // ------------- //

  USceneComponent* RootScene;

  UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "InteractivitySettings")
  UIconComponent* InteractivityIcon;

  // ------------------ //
  // Object Statisctics //
  // ------------------ //

  // Statisctics in form of strings
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterSettings | Statistics")
  TMap<int32, FName> StringStats =
  {
    {ObjectNameStatID, DefaultStringValue},
  };

  // Statisctics in form of integers
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterSettings | Statistics")
  TMap<int32, int32> IntegerStats;

  // ------------------ //
  // CTs System Support //
  // ------------------ //

  // Manager is a way for objects to find other Interactive objects to influence.
  ARegistrationManager* MainManager = nullptr;

  // Node for tag system.
  // Any object can be added only to one system of tags
  // (For multiple tag systems object-wrapper should be used).
  std::shared_ptr<Node<AInteractiveObject>> NodeForCT = nullptr;

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "InteractivitySettings")
  TArray<int32> CTsOfObject;

  // Array of effects which are applied in the current state.
  TArray<UEffectData*> AccumulatedEffects;

protected:
  // Called when the game starts or when spawned.
  virtual void BeginPlay() override;

public:
  AInteractiveObject();

  // ------------------------ //
  // Actor functions overload //
  // ------------------------ //

  // Called before construction script.
  virtual void OnConstruction(const FTransform & Transform) override;

  // Called before construction script.
  virtual void PostInitProperties() override;

  // Called every frame.
	virtual void Tick(float DeltaTime) override;

  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

  // ----------------------- //
  // Connection with Manager //
  // ----------------------- //

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
  // ------ //
  // Others //
  // ------ //

  // Happens when player chooses this object.
  virtual FString GatherInformation() const;

  //Visual interpretation of connections.
  virtual void ShowInfluences() const;

  friend void AddInfluenceOn(AInteractiveObject*);
  friend void RemoveInfluenceFrom(AInteractiveObject*);
  friend class CTsGraph<int32, AInteractiveObject>; // for optimisation purposes
  friend class UBuildAbility;                        // for optimisation purposes
};
