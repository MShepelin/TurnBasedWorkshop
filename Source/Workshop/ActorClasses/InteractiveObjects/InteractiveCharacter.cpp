// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveCharacter.h"
#include "Workshop/UI/TurnBasedEvent/AbilitiesWidget.h"
#include "Components/BillboardComponent.h"
#include "Workshop/UI/AbilitySlot.h"
#include "Kismet/GameplayStatics.h"
#include "InteractiveAbility.h"
#include "Workshop/GameControl/PlayerControllers/InteractController.h"
#include "InteractiveAbility.h"
#include "GenericPlatform/GenericPlatformProcess.h"

AInteractiveCharacter::AInteractiveCharacter()
{
  InteractiveDataCore.InteractiveType = static_cast<int32>(EInteractiveType::Character);

  CharacterPresentation = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("CharacterSprite"));
  CharacterPresentation->SetupAttachment(RootComponent);
  CharacterPresentation->SetRelativeLocation(FVector(0, -1, 0)); // y-order
  CharacterPresentation->SetLooping(false);

  CentralAbilityPositionVisual = CreateDefaultSubobject<UBillboardComponent>(TEXT("CentralAbility"));
  CentralAbilityPositionVisual->SetupAttachment(CollisionBox);

  // Arrange components
  FVector ScaledBoxExtent(CollisionBox->GetScaledBoxExtent());
  CentralAbilityRelativePositionInput = FVector2D(0, ScaledBoxExtent[0]);
}

void AInteractiveCharacter::PlayAnimation(int32 AnimationId)
{
  UPaperFlipbook** FoundFlipbook = CharacterDataCore.AnimationsMap.Find(AnimationId);

  if (FoundFlipbook)
  {
    CharacterPresentation->SetFlipbook(*FoundFlipbook);
    FPlatformProcess::Sleep(CharacterPresentation->GetFlipbookLength());
    CharacterPresentation->SetFlipbook(*CharacterDataCore.AnimationsMap.Find(IdleAnimation));
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("Animation not found!"));
  }
}

void AInteractiveCharacter::PostInitProperties()
{
  Super::PostInitProperties();

  if (!CharacterDataCore.AnimationsMap.Find(IdleAnimation))
  {
    UE_LOG(LogTemp, Error, TEXT("%d animation id (IdleAnimation) must be set"), IdleAnimation);
  }
}

void AInteractiveCharacter::PostEditChangeProperty(struct FPropertyChangedEvent& ChangeEvent)
{
  Super::PostEditChangeProperty(ChangeEvent);
}

void AInteractiveCharacter::OnConstruction(const FTransform & Transform)
{
  Super::OnConstruction(Transform);

  CentralAbilityRelativePosition = FVector(
    CentralAbilityRelativePositionInput[0], WidgetComponentYOrder, CentralAbilityRelativePositionInput[1]);
  CentralAbilityPositionVisual->SetRelativeLocation(CentralAbilityRelativePosition);
}

void AInteractiveCharacter::PickedAsCentral()
{
  Super::PickedAsCentral();

  // Add list of abilities to the AbilitiesWidget.
  UAbilitiesWidget* AbilitiesWidget = Cast<AInteractController>(
    UGameplayStatics::GetPlayerController(GetWorld(), 0))->GetAbilitiesWidget();
  if (AbilitiesWidget)
  {
    AbilitiesWidget->FillAbilitySlots(Abilities, MainManager);
    AbilitiesWidget->FillBarSlots(InteractiveDataCore.Stats);

    AbilitiesWidget->ShowAbilitySlots();
    AbilitiesWidget->ShowBarsSlots();
  }
}

void AInteractiveCharacter::UnpickedAsCentral()
{
  Super::UnpickedAsCentral();

  // Remove list of abilities from the AbilitiesWidget.
  UAbilitiesWidget* AbilitiesWidget = Cast<AInteractController>(
    UGameplayStatics::GetPlayerController(GetWorld(), 0))->GetAbilitiesWidget();
  if (AbilitiesWidget)
  {
    AbilitiesWidget->HideAbilitySlots();
    AbilitiesWidget->HideBarsSlots();
  }
}

void AInteractiveCharacter::BeginPlay()
{
  Super::BeginPlay();
}

void AInteractiveCharacter::SetCentralAbility(AInteractiveAbility* Ability) 
{
  ClearCentralAbility();

  CentralAbility = Ability;
  CentralAbility->SetActorLocation(CentralAbilityRelativePosition + CollisionBox->GetComponentLocation());
  CentralAbility->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

  CentralAbility->SetActorHiddenInGame(false);
}

void AInteractiveCharacter::ChangeCentralAbilityVisibility(bool bIsVisible) //++++ change function name
{
  check(CentralAbility != nullptr);
  CentralAbility->SetActorHiddenInGame(!bIsVisible);
}

void AInteractiveCharacter::RefreshInteractive()
{
  Super::RefreshInteractive();

  // ----------------- //
  // Refresh animation //
  // ----------------- //
  if (!CharacterDataCore.AnimationsMap.Find(IdleAnimation) || !CharacterDataCore.AnimationsMap.Find(ExhaustAnimation))
  {
    return;
  }

  CharacterPresentation->SetFlipbook(CharacterDataCore.AnimationsMap[IdleAnimation]);

  // ----------- //
  // Set y-order //
  // ----------- //

  FVector PresentationLocation = CharacterPresentation->GetRelativeLocation();
  CharacterPresentation->SetRelativeLocation(FVector(
    PresentationLocation[0], MainSpriteYOrder, PresentationLocation[2]));

  // ----------------- //
  // Refresh abilities //
  // ----------------- //
  ACameraController* CurrentController = Cast<ACameraController>(UGameplayStatics::GetPlayerController(this, 0));

  if (!CurrentController || !CurrentController->GetCurrentCamera())
  {
    return;
  }

  Abilities.Empty();

  FVector HiddenLocation = CurrentController->GetCurrentCamera()->GetHiddenLocation();

  for (TSubclassOf<AInteractiveAbility> AbilityClass : CharacterDataCore.AbilitiesClasses)
  {
    AInteractiveAbility* AbilityObject =
      GetWorld()->SpawnActor<AInteractiveAbility>(
        AbilityClass, HiddenLocation, FRotator(0, 0, 0));
    AbilityObject->SetCharacterOwner(this);
    Abilities.Add(AbilityObject);
  }
}

void AInteractiveCharacter::ClearCentralAbility()
{
  if (!CentralAbility)
  {
    return;
  }

  if (CentralAbility->IsCentralInManager())
  {
    CentralAbility->UnpickedAsCentral();
  }

  CentralAbility->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

  CentralAbility->ClearDependencies();
  CentralAbility->ClearInflunces();
  ChangeCentralAbilityVisibility(false);

  ACameraController* CurrentController = Cast<ACameraController>(UGameplayStatics::GetPlayerController(this, 0));
  FVector HiddenLocation = CurrentController->GetCurrentCamera()->GetHiddenLocation(); //???? may be save hidden location somewhere
  CentralAbility->SetActorLocation(HiddenLocation);
}

void AInteractiveCharacter::ResolveCharacterActions()
{
  if (!CentralAbility || CharacterDataCore.bIsExhausted)
  {
    return;
  }

  //++++ add movement
  //ClearCentralAbility();
  CentralAbility->ResolveAbility();
  //++++ add movement
}

void AInteractiveCharacter::UpdateCharacterStatus()
{
  size_t ActiveBarsCounter = 0;
  for (FBar& Stat : InteractiveDataCore.Stats)
  {
    ActiveBarsCounter += Stat.bIsActive;
  }

  if (ActiveBarsCounter == InteractiveDataCore.Stats.Num())
  {
    // Remove from fight
    PlayAnimation(ExhaustAnimation);
    CharacterDataCore.bIsExhausted = true;
  }
}

void AInteractiveCharacter::Pick()
{
  if (CharacterDataCore.bIsExhausted)
  {
    return;
  }

  Super::Pick();
}
