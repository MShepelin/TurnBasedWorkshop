// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "Templates/SharedPointer.h"
#include "InteractiveCharacter.h"
#include "Workshop/Types/InteractiveObjectData/AbilityCore.h"
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

  // ---------------- //
  // Owning character //
  // ---------------- //

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

  virtual void PickedAsTarget() override;

  virtual void UnpickedAsTarget() override;

  // ----------------- //
  // Ability's actions //
  // ----------------- //

  UFUNCTION(BlueprintNativeEvent)
  void CustomEffect(AInteractiveObject* TargetObject);
  virtual void CustomEffect_Implementation(AInteractiveObject* TargetObject);

  // Used when all targets are chosen and effects should be transfered.
  UFUNCTION(BlueprintCallable)
  void ResolveAbility();

  // Decrease duration of temporary effects depending on their resolve phase.
  UFUNCTION(BlueprintCallable)
  void UpdateEffects(ETurnPhase TurnPhase);

  // ------------------- //
  // Ability's variables //
  // ------------------- //

  int32 GetTargetTypeMask() const;

  UTexture2D* GetIconUI() const;

  // ------ //
  // Others //
  // ------ //

  // REMAKE
  // virtual FString GatherInformation() const override;

  void ShowInfluences() const override;

  friend class UBuildAbility; // for optimisation purposes
};
