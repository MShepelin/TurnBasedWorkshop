// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveCharacter.h"


AInteractiveCharacter::AInteractiveCharacter()
{
  InteractiveType = EInteractiveType::Character;

  CharacterPresentation = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("CharacterSprite"));
  CharacterPresentation->SetupAttachment(RootComponent);

  //???? set first default animation?

  CTsOfObject.Add(CharacterOutOfControlCT);
}


FString AInteractiveCharacter::GatherInformation() const
{
  FString CharacterInformation = Super::GatherInformation();

  for (TTuple<int32, int32> IntegerStatPair : IntegerStats)
  {
    CharacterInformation += MainManager->GetStatNameByID(IntegerStatPair.Key) + " " + FString::FromInt(IntegerStatPair.Value) + "\n";
  }

  for (TTuple<int32, FName> StringStatPair : StringStats)
  {
    CharacterInformation += MainManager->GetStatNameByID(StringStatPair.Key) + " " + StringStatPair.Value.ToString() + "\n";
  }

  return CharacterInformation + "\n";
}


void AInteractiveCharacter::ShowInfluences() const
{

}

void AInteractiveCharacter::SetTurn(ETurnPhase TurnPhase)
{
  Super::SetTurn(TurnPhase);

  switch (TurnPhase)
  {
  case ETurnPhase::Start:
    // Decrease effects' duration
    for (size_t EffectIndex = 0; EffectIndex < AccumulatedEffects.Num(); EffectIndex++)
    {
      AccumulatedEffects[EffectIndex]->DecreaseDuration();
      if (!AccumulatedEffects[EffectIndex]->Duration)
      {
        RemoveEffectByIndex(EffectIndex);
      }
    }
    break;

  case ETurnPhase::End:
    break;

  default:
    break;
  }

}

void AInteractiveCharacter::RemoveEffectByIndex(int32 EffectIndex)
{
  AccumulatedEffects.Swap(EffectIndex, AccumulatedEffects.Num() - 1);
  AccumulatedEffects.Pop();
}


void AInteractiveCharacter::PlayAnimation(int32 AnimationId)
{
  UPaperFlipbook** FoundFlipbook = AnimationsMap.Find(AnimationId);

  if (FoundFlipbook)
  {
    CharacterPresentation->SetFlipbook(*FoundFlipbook);
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("Animation not found!"));
  }
}


void AInteractiveCharacter::PostInitProperties()
{
  Super::PostInitProperties();

  // FOR DEBUG ONLY ->
  bool bPlayerControlledCTFound = false;
  bool bOutOfControlCTFound = false;

  for (int32 CT : CTsOfObject)
  {
    if (CT == PlayerControlledCharacterCT)
    {
      bPlayerControlledCTFound = true;
    }
    else if (CT == CharacterOutOfControlCT)
    {
      bOutOfControlCTFound = true;
    }
  }

  if (bOutOfControlCTFound == bPlayerControlledCTFound)
  {
    UE_LOG(LogTemp, Error, TEXT("Every character must be either controlled by player or not, which must be shown in CTs"));
  }
  // FOR DEBUG ONLY <-
}


void AInteractiveCharacter::RemoveEffectsBySpecifiersMask(int32 SpecifiersMask)
{
  for (size_t EffectIndex = 0; EffectIndex < AccumulatedEffects.Num(); EffectIndex++)
  {
    if (AccumulatedEffects[EffectIndex]->EffectSpecifiersMask & SpecifiersMask)
    {
      RemoveEffectByIndex(EffectIndex);
    }
  }
}
