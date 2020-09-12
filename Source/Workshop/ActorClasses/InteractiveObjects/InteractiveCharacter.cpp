// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveCharacter.h"
#include "Workshop/UI/TurnBasedEvent/InformationWidget.h"
#include "Workshop/UI/TurnBasedEvent/AbilitiesWidget.h"
#include "Components/BillboardComponent.h"
#include "Workshop/UI/AbilitySlot.h"
#include "Kismet/GameplayStatics.h"
#include "InteractiveAbility.h"
#include "Workshop/UI/TurnBasedEvent/TurnBasedHUD.h"

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
  CentralAbilityRelativePositionInput = FVector2D(0, UnscaledBoxExtent[0]);
  

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

  if (!AnimationsMap.Find(IdleAnimation))
  {
    UE_LOG(LogTemp, Error, TEXT("%d animation id (IdleAnimation) must be set"), IdleAnimation);
  }

  /*
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
#endif*/
}

void AInteractiveCharacter::PostEditChangeProperty(struct FPropertyChangedEvent& ChangeEvent)
{
  Super::PostEditChangeProperty(ChangeEvent);
}

void AInteractiveCharacter::OnConstruction(const FTransform & Transform)
{
  Super::OnConstruction(Transform);

  CentralAbilityRelativePosition = FVector(
    CentralAbilityRelativePositionInput[0], WidgetComponent, CentralAbilityRelativePositionInput[1]);

#if WITH_EDITOR
  if (!AnimationsMap.Find(IdleAnimation))
  {
    return;
  }
#endif

  CharacterPresentation->SetFlipbook(AnimationsMap[IdleAnimation]);

  FVector ScaledBoxExtent(CollisionBox->GetUnscaledBoxExtent());
  CollisionBox->SetBoxExtent(FVector(ScaledBoxExtent[0], CollisionBoxWidth, ScaledBoxExtent[2]));

  // Set y-order
  CentralAbilityPositionVisual->SetRelativeLocation(CentralAbilityRelativePosition);

  FVector PresentationLocation = CharacterPresentation->GetRelativeLocation();
  CharacterPresentation->SetRelativeLocation(FVector(
    PresentationLocation[0], MainSprite, PresentationLocation[2]));

  FVector BoxLocation = CollisionBox->GetRelativeLocation();
  CollisionBox->SetRelativeLocation(FVector(
    BoxLocation[0], MainSprite, BoxLocation[2]));
}

//int32 AInteractiveCharacter::GetProtectionFromMask() const
//{
//  return ProtectionFrom;
//}

void AInteractiveCharacter::PickedAsCentral()
{
  Super::PickedAsCentral();

  // Add list of abilities to the HUD.
  ATurnBasedHUD* HUD = Cast<ATurnBasedHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
  UAbilitiesWidget* AbilitiesWidget = HUD->GetAbilitiesWidget();
  AbilitiesWidget->SetVisibility(ESlateVisibility::Visible);
  if (AbilitiesWidget)
  {
    AbilitiesWidget->FillAbilitySlots(Abilities);
  }
}

void AInteractiveCharacter::UnpickedAsCentral()
{
  Super::UnpickedAsCentral();

  // Remove list of abilities from the HUD.
  ATurnBasedHUD* HUD = Cast<ATurnBasedHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
  UAbilitiesWidget* AbilitiesWidget = HUD->GetAbilitiesWidget();
  if (AbilitiesWidget)
  {
    AbilitiesWidget->SetVisibility(ESlateVisibility::Hidden);
  }
}

void AInteractiveCharacter::BeginPlay()
{
  Super::BeginPlay();
  
  if (!GetWorld())
  {
    return;
  }

  for (TSubclassOf<AInteractiveAbility> AbilityClass : AbilitiesClasses)
  {
    AInteractiveAbility* AbilityObject = 
      GetWorld()->SpawnActor<AInteractiveAbility>(
        AbilityClass, FVector(0, 0, 0), FRotator(0, 0, 0)); // get controller -> get abilities save location
    AbilityObject->SetCharacterOwner(this);
    AbilityObject->SetActorHiddenInGame(true);
    Abilities.Add(AbilityObject);
  }
}

void AInteractiveCharacter::SetCentralAbility(AInteractiveAbility* Ability) 
{
  if (CentralAbility)
  {
    if (CentralAbility->IsCentral())
    {
      CentralAbility->UnpickedAsCentral();
    }

    CentralAbility->ClearDependencies();
    CentralAbility->ClearInflunces();
    SetCentralAbilityVisibility(true);
    CentralAbility->SetActorLocation(FVector(0, 0, 0)); // get controller -> get abilities save location
  }

  CentralAbility = Ability;
  CentralAbility->SetActorLocation(CentralAbilityRelativePosition + CollisionBox->GetComponentLocation());
}

void AInteractiveCharacter::SetCentralAbilityVisibility(bool bIsInvisible)
{
  check(CentralAbility != nullptr);
  CentralAbility->SetActorHiddenInGame(bIsInvisible);
}
