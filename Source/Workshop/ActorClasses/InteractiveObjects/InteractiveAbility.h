// Copyright © Dmitriy Shepelin 2021. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "Templates/SharedPointer.h"
#include "InteractiveCharacter.h"
#include "Workshop/Types/InteractiveObjectData/AbilityCore.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "InteractiveAbility.generated.h"

class AInteractiveCharacter;

/**
 * Abilities store and resolve effects. They are held by InteractiveCharacters who determine where abilities are spawned on a level.
 * In addition, abilities have visuals and some native events free for implementation.
 */
UCLASS(Abstract, Blueprintable)
class WORKSHOP_API AInteractiveAbility : public AInteractiveObject
{
  GENERATED_BODY()

protected:
  // ------- //
  // Visuals //
  // ------- //

  UPROPERTY(VisibleDefaultsOnly)
  UPaperFlipbookComponent* AbilityPresentation;

  /** Pins represent the number of targets that the player can choose. */
  UPROPERTY(EditDefaultsOnly)
  UInstancedStaticMeshComponent* TargetsPins;

  /** Used to create instances for TargetsPins in a certain direction. */
  UPROPERTY(EditDefaultsOnly)
  FVector PinSpawnDirection;

  /** Transform of the first instance of TargetsPins. */
  UPROPERTY(EditDefaultsOnly)
  FTransform FirstPinLocalTransform;

  // ---------------- //
  // Owning character //
  // ---------------- //

  UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
  AInteractiveCharacter* CharacterOwner;

public:
  /** Variables used by the ability to affect objects. */
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

  /** Used to initialise character-owner. */
  UFUNCTION() void SetCharacterOwner(AInteractiveCharacter* NewCharacterOwner);

  /** 
   * Used to mark this ability as central for the character-owner.
   * It's useful if a player needs to choose one ability for each InteractiveCharacter. 
   */
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

  /** 
   * Defines what happens when the ability must be resolved.
   * 1) Use BuildAbility function library to create implementation
   * 2) To get all chosen targets use GetTargets from BuildAbility
   * 3) Implementation must be thead-safe
   */
  UFUNCTION(BlueprintNativeEvent)
  void ResolveAbility();
  virtual void ResolveAbility_Implementation();

  /**
   * Reset the counter of the number of available targets.
   * It only changes the counter and visuals without editting the influence on other objects.
   */
  UFUNCTION(BlueprintCallable)
  void ResetAvailableTargets();

  /** 
   * To resolve the ability and implement transportation of needed effects outside of the main thread
   * we should prepare the ability and put it's effects to thread-safe resources. 
   * This functions is not thread-safe.
   */
  void PrepareToResolve();

  /**
   * After resolution step we need to clear used resources.
   * This function is expected to be used after resoultion step. This functions is not thread-safe.
   */
  void UpdateAfterResolution();

  /** Decreases duration of bonus effects applied to the ability. */
  void DecreaseEffectsDuration();

  // ------------------- //
  // Ability's variables //
  // ------------------- //

  /**
   * Abilities use CTs to determine available targets but also they can use type masks.
   * CTs and type masks are both considered when possible targets are searched.
   */
  int32 GetTargetTypeMask() const;

  /** Get the icon which the ability uses for visualisation. */
  UTexture2D* GetIconUI() const;

  // ------ //
  // Others //
  // ------ //

#if WITH_EDITOR
  void ShowInfluences() const override;
#endif

  /**
   * BuildAbility gives prepared functions which help to make new actions for resolution step with ResolveAbility.
   * For optimisation purposes BuildAbility was designed to work with private members of abilities.
   */
  friend class UBuildAbility;
};
