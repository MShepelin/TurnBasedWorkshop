// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Workshop/Types/Nonblueprintable/GameConstants.h"
#include "Workshop/Types/Nonblueprintable/TagsSystem.h"
#include "Workshop/Types/Components/IconComponent.h"
#include "Workshop/Types/InteractiveType.h"
#include "GameFramework/Actor.h"
#include <unordered_map>
#include "DrawDebugHelpers.h"
#include "InteractiveObject.generated.h"


class UBuildAbility;
class UEffectData;
class ARegistrationManager;


// Interactive object supports tag typization and dependecy from other interactive objects
// Although it doesn't listen to other entity's interaction there are functions to
// consider external actions
UCLASS(Blueprintable)
class WORKSHOP_API AInteractiveObject : public AActor
{
	GENERATED_BODY()

protected:
  TSet<AInteractiveObject*> DependenciesList;
  TSet<AInteractiveObject*> InfluencesList;

  EInteractiveType InteractiveType = EInteractiveType::Base;

  USceneComponent* RootScene;

  UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "InteractivitySettings")
  UIconComponent* InteractivityIcon;
  
public:
	// Sets default values for this actor's properties
	AInteractiveObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
  
  ARegistrationManager* MainManager = nullptr;

  // Node for tag system
  // Any object can be added only to one system of tags
  // (For multiple tag systems object-decorator should be used)
  std::shared_ptr<Node<AInteractiveObject>> NodeForTags = nullptr;

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "InteractivitySettings")
  TArray<int32> TagsForTypizationSearch;

  std::shared_ptr<Node<AInteractiveObject>>&    GetNode();
  TArray<int32>&                                GetTags();

  friend class TagsGraph<int32, AInteractiveObject>;


  // List of effects which are applied in the current state
  TArray<UEffectData*> AccumulatedEffects;

  friend class UBuildAbility;

public:
  //Called before construction script.
  virtual void OnConstruction(const FTransform & Transform) override;

  virtual void PostInitProperties() override;

  // Called every frame.
	virtual void Tick(float DeltaTime) override;


  void ConnectToManager(ARegistrationManager* Manager);


  UFUNCTION(BlueprintCallable)
  void AddInfluenceOn(AInteractiveObject* object);

  UFUNCTION(BlueprintCallable)
  void RemoveDependenceFrom(AInteractiveObject * object);

  UFUNCTION(BlueprintCallable)
  void ClearInflunces();

  UFUNCTION(BlueprintCallable)
  void ClearDependencies();

  friend void AddInfluenceOn(AInteractiveObject*);
  friend void RemoveInfluenceFrom(AInteractiveObject*);


  // Happens when player chooses this object.
  virtual FString GatherInformation() const;

  //Visual interpretation of connections.
  virtual void ShowInfluences() const;
};
