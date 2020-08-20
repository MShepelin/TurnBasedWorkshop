// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "../IconData.h"
#include "IconComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WORKSHOP_API UIconComponent : public USceneComponent
{
	GENERATED_BODY()

protected:
  bool bIsAvailable;

  UPaperFlipbookComponent* SpriteOfIcon;
  UIconData* IconProperties;

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "IconClass")
  TSubclassOf<UIconData> IconPropertiesClass;

public:
	// Sets default values for this component's properties
	UIconComponent();

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

  // Initialize components' visuals, better to be called on Actors' OnConstruction()
  void Initialize();

  UFUNCTION(BlueprintCallable)
  void Show();

  UFUNCTION(BlueprintCallable)
  void Hide();
  
  UFUNCTION(BlueprintCallable)
  void SetAvailability(bool bNewAvailability);

  UFUNCTION(BlueprintCallable)
  bool IsAvailable();
};
