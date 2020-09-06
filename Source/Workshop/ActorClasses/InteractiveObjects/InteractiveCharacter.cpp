// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveCharacter.h"
#include "Workshop/UI/TurnBasedEvent/InformationWidget.h"
#include "Workshop/UI/TurnBasedEvent/AbilitiesWidget.h"
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

  CentralAbilityWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("CentralAbility"));
  CentralAbilityWidgetComponent->SetupAttachment(CollisionBox);

  // Arrange components
  FVector UnscaledBoxExtent(CollisionBox->GetUnscaledBoxExtent());
  CentralAbilityWidgetComponent->SetRelativeLocation(FVector(0, 0, UnscaledBoxExtent[0]));

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
  /*
  FName PropertyName = (ChangeEvent.Property != nullptr) ? ChangeEvent.Property->GetFName() : NAME_None;
  if (PropertyName == GET_MEMBER_NAME_CHECKED(AInteractiveCharacter, WidgetClass))
  {
    UE_LOG(LogTemp, Warning, TEXT("gotcha"));
  }
  */
  Super::PostEditChangeProperty(ChangeEvent);
}

void AInteractiveCharacter::OnConstruction(const FTransform & Transform)
{
  Super::OnConstruction(Transform);

  CentralAbilityWidgetComponent->SetWidgetClass(CentralAbilityWidgetClass);
  CentralAbilityWidgetComponent->SetRelativeRotation(FRotator(0, 90, 0));

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
}

void AInteractiveCharacter::UnpickedAsCentral()
{
  Super::UnpickedAsCentral();
}

void AInteractiveCharacter::BeginPlay()
{
  Super::BeginPlay();
  
  for (TSubclassOf<AInteractiveAbility> AbilityClass : AbilitiesClasses)
  {
    Abilities.Add(NewObject<AInteractiveAbility>(this, AbilityClass));
  }
}

void AInteractiveCharacter::SetCentralAbility(AInteractiveAbility* Ability) 
{
  CentralAbility = Ability;
}

void AInteractiveCharacter::HideCentralWidget()
{
  //CentralAbilityWidget->Hide()
}

void AInteractiveCharacter::ShowCentralWidget()
{
  //CentralAbilityWidget->Show()
}
