// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "../IconState.h"
#include "../IconData.h"
#include "IconComponent.generated.h"


// Icon can show one of 3 states: available or unavailable.
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WORKSHOP_API UIconComponent : public USceneComponent
{
	GENERATED_BODY()

protected:
  UPROPERTY() EIconState IconState;
  UPROPERTY() bool bIsShown;

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
  bool IsShown() const;

  UFUNCTION(BlueprintCallable)
  EIconState GetIconState() const;

  UFUNCTION(BlueprintCallable)
  void SetIconState(EIconState NewIconState);
};
