// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveCharacter.h"


AInteractiveCharacter::AInteractiveCharacter()
{
  InteractiveType = EInteractiveType::Character;

  CharacterPresentation = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("CharacterSprite"));
  CharacterPresentation->SetupAttachment(RootComponent);

  //???? set first default animation?
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


void AInteractiveCharacter::OnTurnStart()
{
  //++++ apply effects
}


void AInteractiveCharacter::RemoveEffectByIndex(int32 EffectIndex)
{
  AccumulatedEffects.Swap(EffectIndex, AccumulatedEffects.Num() - 1);
  AccumulatedEffects.Pop();
}


void AInteractiveCharacter::OnTurnEnd()
{
  // Decrease effects' duration
  for (size_t EffectIndex = 0; EffectIndex < AccumulatedEffects.Num(); EffectIndex++)
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

  //???? check if needed Stats are present
}


void AInteractiveCharacter::RemoveEffectsBySpecifiersMask(int32 SpecifiersMask)
{
  for (size_t EffectIndex = 0; EffectIndex < AccumulatedEffects.Num(); EffectIndex++)
  {
    if (AccumulatedEffects[EffectIndex]->GetEffectSpecifiers() & SpecifiersMask)
    {
      RemoveEffectByIndex(EffectIndex);
    }
  }
}