// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "Templates/SharedPointer.h"
#include "InteractiveCharacter.h"
#include "Workshop/Types/InteractiveObjectData/AbilityCore.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "InteractiveAbility.generated.h"

class AInteractiveCharacter;

// Represenatation of character's gameplay possibilities on game scene.
UCLASS(Abstract, Blueprintable)
class WORKSHOP_API AInteractiveAbility : public AInteractiveObject
{
  //++++ add limit for ability's chosable objects

  GENERATED_BODY()

protected:
  // ------- //
  // Visuals //
  // ------- //

  UPROPERTY(VisibleDefaultsOnly)
  UPaperFlipbookComponent* AbilityPresentation;

  // Pins represent the number of targets that the player can choose.
  UPROPERTY(EditDefaultsOnly)
  UInstancedStaticMeshComponent* TargetsPins;

  // Used to create instances for TargetsPins in a certain direction. 
  UPROPERTY(EditDefaultsOnly)
  FVector PinSpawnDirection;

  // Transform of the first instance of TargetsPins.
  UPROPERTY(EditDefaultsOnly)
  FTransform FirstPinLocalTransform;

  // ---------------- //
  // Owning character //
  // ---------------- //

  UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
  AInteractiveCharacter* CharacterOwner;

public:
  // Variables which ability is using to affect objects.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
  FAbilityCore AbilityDataCore;

  AInteractiveAbility();

  // ------------------------- //
  // AActor functions overload //
  // ------------------------- //

  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

  virtual void PostInitProperties() override;

  virtual void BeginPlay() override;

  // ---------------- //
  // Owning character //
  // ---------------- //

  // Used to initialise character-owner.
  void SetCharacterOwner(AInteractiveCharacter* NewCharacterOwner);

  UFUNCTION() void CenterInCharacterOwner();

  // ----------------------- //
  // Connection with Manager //
  // ----------------------- //

  virtual void PickedAsCentral() override;

  virtual void UnpickedAsCentral() override;

  // --------------------------- //
  // Influences and dependencies //
  // --------------------------- //

  virtual bool AddInfluenceOn(AInteractiveObject* Object) override;

  virtual bool RemoveInfluenceOn(AInteractiveObject* Object) override;

  virtual void ClearInflunces() override;

  // ----------------- //
  // Ability's actions //
  // ----------------- //

  // Defines what happens when the ability must be resolved.
  // 1) Use BuildAbility function library to create implementation
  // 2) To get all chosen targets use GetTargets from BuildAbility
  // 3) Implementation must be thead-safe
  UFUNCTION(BlueprintNativeEvent)
  void ResolveAbility();
  virtual void ResolveAbility_Implementation();

  // Reset the counter of the number of available targets.
  // It only changes the counter and visuals without editting the influence on other objects.
  UFUNCTION(BlueprintCallable)
  void ResetAvailableTargets();

  void PrepareToResolve();

  void UpdateAfterResolution();

  // ------------------- //
  // Ability's variables //
  // ------------------- //

  int32 GetTargetTypeMask() const;

  UTexture2D* GetIconUI() const;

  // ------ //
  // Others //
  // ------ //

  void ShowInfluences() const override;

  friend class UBuildAbility; // for optimisation purposes
};
