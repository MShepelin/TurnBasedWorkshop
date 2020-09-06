// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "Workshop/Types/Effects/EffectData.h"
#include "Components/WidgetComponent.h"
#include "InteractiveObject.h"
#include "Components/BoxComponent.h"
#include "InteractiveCharacter.generated.h"


class AInteractiveAbility;
class UAbilitiesWidget;
class UInformationWidget;
class UAbilitySlot;


// Interactive characters have visual represenation, abilities, types, 
// they are able to take actions in turn-based events and connected to 
// skill activations.
UCLASS(Blueprintable)
class WORKSHOP_API AInteractiveCharacter : public AInteractiveObject
{
	GENERATED_BODY()

protected:
  // --------- //
  // Abilities //
  // --------- //

  // Abilities which Interactive character can use.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterSettings")
  TArray<TSubclassOf<AInteractiveAbility>> AbilitiesClasses;
  UPROPERTY() TArray<AInteractiveAbility*> Abilities;

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterSettings", meta = (Bitmask, BitmaskEnum = "EEffectSpecifiers"))
  int32 ProtectionFrom = 0;

  // ------- //
  // Visuals //
  // ------- //

  // Map of animations with their integer identifiers.
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterSettings")
  TMap<int32, UPaperFlipbook*> AnimationsMap;

  //???? add state machine support functions

  UPROPERTY(VisibleDefaultsOnly)
  UPaperFlipbookComponent* CharacterPresentation;

  UPROPERTY() AInteractiveAbility* CentralAbility;

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterSettings") 
  UWidgetComponent* CentralAbilityWidgetComponent;

  // Changes made in CentralAbilityWidgetComponent's Widget Class will not be applied
  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterSettings")
  TSubclassOf<UAbilitySlot> CentralAbilityWidgetClass;

  UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterSettings")
  UBoxComponent* CollisionBox;

public:
  AInteractiveCharacter();

  // ------------------------ //
  // Actor functions overload //
  // ------------------------ //

  virtual void PostInitProperties() override;

  virtual void BeginPlay() override;

  virtual void OnConstruction(const FTransform & Transform) override;

  virtual void PostEditChangeProperty(struct FPropertyChangedEvent& ChangeEvent);

  // ---------- //
  // Animations //
  // ---------- //

  // Play animation by its ID
  void PlayAnimation(int32 AnimationID);

  // ----------------------- //
  // Connection with Manager //
  // ----------------------- //

  virtual void PickedAsCentral() override;

  virtual void UnpickedAsCentral() override;

  // ------------------ //
  // Turn-based actions //
  // ------------------ //

  virtual void SetTurn(ETurnPhase TurnPhase) override;

  // ----------------------//
  // Character Information //
  // --------------------- //

  int32 GetProtectionFromMask() const;

  // ----------------- //
  // UI with abilities //
  // ----------------- //

  void SetCentralAbility(AInteractiveAbility* Ability);

  UFUNCTION(BlueprintCallable)
  void HideCentralWidget();

  UFUNCTION(BlueprintCallable)
  void ShowCentralWidget();

  // ------ //
  // Others //
  // ------ //

  virtual FString GatherInformation() const override;

  void ShowInfluences() const override;

  friend class UBuildAbility; // for optimisation purposes
};
