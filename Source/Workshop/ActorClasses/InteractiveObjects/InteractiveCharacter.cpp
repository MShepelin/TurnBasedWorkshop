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

void AInteractiveCharacter::ResolveEffects()
{
  for (UEffectData* effect : AppliedEffects)
  {
    // ???
  }
}

void AInteractiveCharacter::OnTurnStart()
{

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
}