// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Workshop/Types/Nonblueprintable/GameConstants.h"
#include "Workshop/Types/InteractiveObjectData/InteractiveCore.h"
#include "Workshop/Types/InteractiveObjectData/CharacterCore.h"
#include "TurnBasedManager.h"
#include "TurnBasedObserver.generated.h"

class ITurnBasedInterface;

UCLASS(Blueprintable)
class WORKSHOP_API ATurnBasedObserver : public APawn
{
  GENERATED_BODY()

  UPROPERTY() TScriptInterface<ITurnBasedInterface> TurnBasedController = nullptr;

protected:
  UPROPERTY() ATurnBasedManager *TurnBasedManager;

  UPROPERTY(EditAnywhere)
  TArray<TSubclassOf<AInteractiveCharacter>> CharactersToUse;

  UPROPERTY() TArray<FCharacterCore> CharactersOptions;

  UPROPERTY() TArray<FInteractiveCore> InteractiveOptions;

  UPROPERTY() TArray<AInteractiveCharacter*> SpawnedCharacters;

public:
  virtual void PossessedBy(AController *NewController) override;

  virtual void UnPossessed() override;

  void OnConnectToManager(ATurnBasedManager *Manager);

  void OnDisconnectFromManager();

  void OnGetTurnControl();

  void OnLoseTurnControl();

  // ----------------------------- //
  // Communication with Controller //
  // ----------------------------- //

  UFUNCTION(BlueprintCallable)
  ATurnBasedManager* GetManager();

  void SetCharactersToUse(
    const TArray<TSubclassOf<AInteractiveCharacter>>& NewCharacterClasses,
    const TArray<FCharacterCore> *NewCharactersOptions = nullptr,
    const TArray<FInteractiveCore> *NewInteractiveOptions = nullptr
  );

  UFUNCTION(BlueprintCallable)
  void SpawnCharacters(FVector HiddenLocation);

  UFUNCTION(BlueprintCallable)
  void RemoveCharacters();
};
