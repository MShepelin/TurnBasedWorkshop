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

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "IconClass")
  FIconData IconProperties;

  // Called when the game starts.
  virtual void BeginPlay() override;

public:
	UIconComponent();

	// Called every frame.
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

  virtual void PostInitProperties() override;

  // Create Icon Components' visuals.
  // Should be called on Actor's OnConstruction().
  UFUNCTION(BlueprintCallable)
  void Initialize();

  UFUNCTION(BlueprintCallable)
  void Show();

  UFUNCTION(BlueprintCallable)
  void Hide();
  
  UFUNCTION(BlueprintCallable)
  void SetAvailability(bool bNewAvailability);

  UFUNCTION(BlueprintCallable)
  bool IsAvailable() const;
};
