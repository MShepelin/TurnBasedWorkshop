// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveCharacter.h"
#include "Workshop/UI/TurnBasedEvent/InformationWidget.h"
#include "Workshop/UI/TurnBasedEvent/AbilitiesWidget.h"
#include "Components/BillboardComponent.h"
#include "Workshop/UI/AbilitySlot.h"
#include "InteractiveAbility.h"

AInteractiveCharacter::AInteractiveCharacter()
{
  InteractiveType = static_cast<int32>(EInteractiveType::Character);

  CharacterPresentation = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("CharacterSprite"));
  CharacterPresentation->SetupAttachment(RootComponent);
  CharacterPresentation->SetRelativeLocation(FVector(0, -1, 0)); // y-order

  CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
  CollisionBox->SetupAttachment(RootComponent);

  CentralAbilityPositionVisual = CreateDefaultSubobject<UBillboardComponent>(TEXT("CentralAbility"));
  CentralAbilityPositionVisual->SetupAttachment(CollisionBox);

  // Arrange components
  FVector UnscaledBoxExtent(CollisionBox->GetUnscaledBoxExtent());
  CentralAbilityRelativePosition = FVector(0, 0, UnscaledBoxExtent[0]);
  CentralAbilityPositionVisual->SetRelativeLocation(CentralAbilityRelativePosition);

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
  Super::ShowInfluences();
}

void AInteractiveCharacter::SetTurn(ETurnPhase TurnPhase)
{
  for (size_t EffectIndex = 0; EffectIndex < AccumulatedEffects.Num(); EffectIndex++)
  {
    UEffectData* ChosenEffect = AccumulatedEffects[EffectIndex];

    if (ChosenEffect->TurnPhaseToResolve != TurnPhase)
    {
      continue;
    }

    ChosenEffect->ResolveOn(this);
    ChosenEffect->DecreaseDuration();

    // Remove effect if it is no longer present
    if (!ChosenEffect->Duration)
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

#if WITH_EDITOR
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
#endif
}

void AInteractiveCharacter::PostEditChangeProperty(struct FPropertyChangedEvent& ChangeEvent)
{
  Super::PostEditChangeProperty(ChangeEvent);
}

void AInteractiveCharacter::OnConstruction(const FTransform & Transform)
{
  Super::OnConstruction(Transform);

  CentralAbilityPositionVisual->SetRelativeLocation(CentralAbilityRelativePosition);

  FVector ScaledBoxExtent(CollisionBox->GetUnscaledBoxExtent());
  CollisionBox->SetBoxExtent(FVector(ScaledBoxExtent[0], CollisionBoxWidth, ScaledBoxExtent[2]));
}

int32 AInteractiveCharacter::GetProtectionFromMask() const
{
  return ProtectionFrom;
}

void AInteractiveCharacter::PickedAsCentral()
{
  Super::PickedAsCentral();

  //++++ add list of abilities to hud
}

void AInteractiveCharacter::UnpickedAsCentral()
{
  Super::UnpickedAsCentral();

  //++++ remove list of abilities from hud
}

void AInteractiveCharacter::BeginPlay()
{
  Super::BeginPlay();
  
  for (TSubclassOf<AInteractiveAbility> AbilityClass : AbilitiesClasses)
  {
    AInteractiveAbility* AbilityObject = NewObject<AInteractiveAbility>(this, AbilityClass);
    AbilityObject->SetActorLocation(FVector(0, 0, 0)); // get controller -> get abilities save location
    AbilityObject->SetOwner(this);
    Abilities.Add(AbilityObject);
  }
}

void AInteractiveCharacter::SetCentralAbility(AInteractiveAbility* Ability) 
{
  if (CentralAbility)
  {
    SetCentralAbilityVisibility(false);
    CentralAbility->SetActorLocation(FVector(0, 0, 0)); // get controller -> get abilities save location
  }

  CentralAbility = Ability;
  CentralAbility->SetActorRelativeLocation(CentralAbilityRelativePosition);
}

void AInteractiveCharacter::SetCentralAbilityVisibility(bool bIsVisible)
{
  check(CentralAbility != nullptr);
  CentralAbility->SetActorHiddenInGame(bIsVisible);
}
