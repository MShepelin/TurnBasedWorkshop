// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Workshop/ActorClasses/InteractiveObjects/InteractiveObject.h"
#include "RegistrationManager.generated.h"


// Used for drawing lines, spheres and other shapes for debug only
const FColor DebugColor(255, 0, 0);

/**
 * Manager connects Interactive objects, supports their references, provides search by tags.
 */
UCLASS()
class WORKSHOP_API ARegistrationManager : public AActor
{
	GENERATED_BODY()
	
public:
  //++++ debugicon

  // Here are all tags available in this manager
  UPROPERTY(BlueprintReadWrite /*BlueprintReadOnly*/, EditDefaultsOnly)
  TMap<int32, FString> TagsWithNames;

  // System of tags
  TagsGraph<int32, AInteractiveObject> TagsSystem;

	// Sets default values for this actor's properties
	ARegistrationManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

  void PostInitProperties() override;

  UFUNCTION(BlueprintCallable)
  void ConnectObjectToTagsSystem(AInteractiveObject* ObjectToAdd);

  UFUNCTION(BlueprintCallable)
  void DisconnectObjectFromTagsSystem(AInteractiveObject* ObjectToRemove);

  UFUNCTION(BlueprintCallable)
  TArray<AInteractiveObject*> FindObjectsByTags(const TArray<int32> TagsArray, int32 EnoughNumberOfTags) const;
};
