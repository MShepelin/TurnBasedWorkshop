// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Workshop/Types/Components/IconComponent.h"
#include "Workshop/Types/InteractiveType.h"
#include "GameFramework/Actor.h"
#include <unordered_map>
#include "InteractiveObject.generated.h"

// Base, can be clicked
//++++ add a little bit more info
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
  
public:
  // Called before construction script
  virtual void OnConstruction(const FTransform & Transform) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

  // ---------------------------------------------------------------
  //                      Influence Management
  // ---------------------------------------------------------------

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

  // ---------------------------------------------------------------
  //                   Get information about object
  // ---------------------------------------------------------------

  // Happens when player chooses this object
  //++++ returns json to get current information
  virtual void GatherInformation();

  // Visual interpretation of connections
  virtual void ShowInfluences();
};
