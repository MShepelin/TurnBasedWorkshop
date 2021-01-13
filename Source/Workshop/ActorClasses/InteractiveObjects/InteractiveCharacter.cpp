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
  CharacterPresentation->SetLooping(true);

  CentralAbilityPositionVisual = CreateDefaultSubobject<UBillboardComponent>(TEXT("CentralAbility"));
  CentralAbilityPositionVisual->SetupAttachment(CollisionBox);

  // Arrange components
  FVector ScaledBoxExtent(CollisionBox->GetScaledBoxExtent());
  CentralAbilityRelativePositionInput = FVector2D(0, ScaledBoxExtent[0]);
}

void AInteractiveCharacter::PlayAnimation(int32 AnimationId, bool bWaitUntilEnds)
{
  UPaperFlipbook** FoundFlipbook = CharacterDataCore.AnimationsMap.Find(AnimationId);
  if (FoundFlipbook && *FoundFlipbook)
  {
    CharacterDataCore.AnimationQueue.Enqueue(*FoundFlipbook);

    if (bWaitUntilEnds)
      FPlatformProcess::Sleep((**FoundFlipbook).GetTotalDuration() * SLEEP_MULTIPLIER);
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("Animation not found!"));
  }
}

void AInteractiveCharacter::PostInitProperties()
{
  Super::PostInitProperties();

  if (!CharacterDataCore.AnimationsMap.Find(IDLE_ANIMATION_ID))
  {
    UE_LOG(LogTemp, Error, TEXT("%d animation id (IDLE_ANIMATION_ID) must be set"), IDLE_ANIMATION_ID);
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
    CentralAbilityRelativePositionInput[0], WIDGET_Y_ORDER, CentralAbilityRelativePositionInput[1]);
  CentralAbilityPositionVisual->SetRelativeLocation(CentralAbilityRelativePosition);
}

void AInteractiveCharacter::PickedAsCentral()
{
  Super::PickedAsCentral();

  int32 CheckControll = InteractiveDataCore.InteractiveType & static_cast<int32>(EInteractiveType::PlayerControlled);

  // Add list of abilities to the AbilitiesWidget.
  UAbilitiesWidget* AbilitiesWidget = Cast<AInteractController>(
    UGameplayStatics::GetPlayerController(GetWorld(), 0))->GetAbilitiesWidget();
  if (AbilitiesWidget)
  {
    AbilitiesWidget->FillAbilitySlots(Abilities, MainManager, CheckControll);
    AbilitiesWidget->FillBarSlots(InteractiveDataCore.Stats, MainManager);

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
  if (!CharacterDataCore.AnimationsMap.Find(IDLE_ANIMATION_ID) || !CharacterDataCore.AnimationsMap.Find(EXHAUST_ANIMATION_ID))
  {
    return;
  }

  ResetAnimation();

  // ----------- //
  // Set y-order //
  // ----------- //

  FVector PresentationLocation = CharacterPresentation->GetRelativeLocation();
  CharacterPresentation->SetRelativeLocation(FVector(
    PresentationLocation[0], SPRITE_Y_ORDER, PresentationLocation[2]));

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
    if (!CharacterDataCore.bIsExhausted)
      UpdateCharacterStatus();
    return;
  }

  //++++ add movement
  //ClearCentralAbility();
  CentralAbility->ResolveAbility();
  UpdateCharacterStatus();
  //++++ add movement
}

void AInteractiveCharacter::UpdateCharacterStatus()
{
  ResetAnimation();
  size_t ActiveBarsCounter = 0;
  for (FBar& Stat : InteractiveDataCore.GetStats())
  {
    if (Stat.bIsActive)
      ActiveBarsCounter++;
  }

  if (ActiveBarsCounter == InteractiveDataCore.Stats.Num())
  {
    PlayAnimation(EXHAUST_ANIMATION_ID, false);
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

void AInteractiveCharacter::ResetAnimation()
{
  PlayAnimation(IDLE_ANIMATION_ID, false);
}

void AInteractiveCharacter::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  UPaperFlipbook* FoundFlipbook = nullptr;
  while (CharacterDataCore.AnimationQueue.Dequeue(FoundFlipbook)) {};

  if (FoundFlipbook)
    CharacterPresentation->SetFlipbook(FoundFlipbook);
}
