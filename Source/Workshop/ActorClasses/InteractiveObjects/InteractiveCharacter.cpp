// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveCharacter.h"


AInteractiveCharacter::AInteractiveCharacter()
{
  CharacterPresentation = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("CharacterSprite"));
  CharacterPresentation->SetupAttachment(RootComponent);

  // Set first default animation?
}


void AInteractiveCharacter::GatherInformation()
{
  Super::GatherInformation();
}


void AInteractiveCharacter::ShowInfluences()
{

}


void AInteractiveCharacter::OnTurnStart()
{
  // Apply effects
}


void AInteractiveCharacter::RemoveEffectByIndex(int EffectIndex)
{
  AccumulatedEffects.Swap(EffectIndex, AccumulatedEffects.Num() - 1);
  AccumulatedEffects.Pop();
}

void AInteractiveCharacter::OnTurnEnd()
{
  // Decrease effects' duration
  for (int EffectIndex = 0; EffectIndex < AccumulatedEffects.Num(); EffectIndex++)
  {
    AccumulatedEffects[EffectIndex]->DecreaseDuration();
    if (!AccumulatedEffects[EffectIndex]->GetDuration())
    {
      RemoveEffectByIndex(EffectIndex);
    }
  }
}


void AInteractiveCharacter::PlayAnimation(int32 AnimationId)
{
  UPaperFlipbook** FoundFlipbook = AnimationsCollection.Find(AnimationId);
  if (FoundFlipbook)
  {
    CharacterPresentation->SetFlipbook(*FoundFlipbook);
  }
  else
  {
    UE_LOG(LogTemp, Warning, TEXT("Animation not found!"));
  }
}


void AInteractiveCharacter::OnConstruction(const FTransform & Transform)
{
  Super::OnConstruction(Transform);

  // Check if needed Stats are present
  if (!StringStats.Find(CharacterNameStatId))
  {
    UE_LOG(LogTemp, Warning, TEXT("Incorrect Character Stats!"));
  }
}

void AInteractiveCharacter::RemoveEffectsBySpecifiersMask(int32 mask)
{
  for (int EffectIndex = 0; EffectIndex < AccumulatedEffects.Num(); EffectIndex++)
  {
    if (AccumulatedEffects[EffectIndex]->GetEffectSpecifiers() & mask)
    {
      RemoveEffectByIndex(EffectIndex);
    }
  }
}