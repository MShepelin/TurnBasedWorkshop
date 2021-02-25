// Copyright © Dmitriy Shepelin 2021. MIT License.

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
  InteractiveDataCore.InteractiveType = static_cast<uint8>(EInteractiveType::Character);

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
    float AnimationDuration = (**FoundFlipbook).GetTotalDuration();
    CharacterDataCore.AnimationQueue.Enqueue(TTuple<UPaperFlipbook*, float, int32>{ *FoundFlipbook, AnimationDuration, AnimationId });

    if (bWaitUntilEnds)
      FPlatformProcess::Sleep(THREAD_SLEEP(AnimationDuration));
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("Animation not found!"));
  }
}

void AInteractiveCharacter::OnConstruction(const FTransform & Transform)
{
  Super::OnConstruction(Transform);

  if (!CharacterDataCore.AnimationsMap.Find(IDLE_ANIMATION_ID))
  {
    UE_LOG(LogTemp, Warning, TEXT("%d animation id (IDLE_ANIMATION_ID) must be set"), IDLE_ANIMATION_ID);
  }

  CentralAbilityRelativePosition = FVector(
    CentralAbilityRelativePositionInput[0], WIDGET_Y_ORDER, CentralAbilityRelativePositionInput[1]);
  CentralAbilityPositionVisual->SetRelativeLocation(CentralAbilityRelativePosition);
}

void AInteractiveCharacter::PickedAsCentral()
{
  Super::PickedAsCentral();

  uint8 CheckControll = InteractiveDataCore.InteractiveType & static_cast<uint8>(EInteractiveType::PlayerControlled);

  // Add list of abilities to the AbilitiesWidget.
  UAbilitiesWidget* AbilitiesWidget = Cast<AInteractController>(
    UGameplayStatics::GetPlayerController(GetWorld(), 0))->GetAbilitiesWidget();
  if (AbilitiesWidget)
  {
    AbilitiesWidget->FillAbilitySlots(Abilities, CheckControll);
    AbilitiesWidget->FillBarSlots(InteractiveDataCore.Stats);

    AbilitiesWidget->ShowAbilitySlots();
    AbilitiesWidget->ShowBarsSlots();

    AbilitiesWidget->SetInteractiveObjectData(InteractiveDataCore);
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
    AbilitiesWidget->HideName();
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

void AInteractiveCharacter::BuildInteractive()
{
  Super::BuildInteractive();

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

void AInteractiveCharacter::ClearCentralAbility(bool bOnlyVisually)
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

  ChangeCentralAbilityVisibility(false);

  ACameraController* CurrentController = Cast<ACameraController>(UGameplayStatics::GetPlayerController(this, 0));
  FVector HiddenLocation = CurrentController->GetCurrentCamera()->GetHiddenLocation(); //???? may be save hidden location somewhere
  CentralAbility->SetActorLocation(HiddenLocation);

  if (!bOnlyVisually)
  {
    CentralAbility->ClearDependencies();
    CentralAbility->ClearInflunces();
    CentralAbility = nullptr;
  }
}

void AInteractiveCharacter::ResolveCharacterActions()
{
  if (CharacterDataCore.bIsExhausted)
  {
    return;
  }

  if (CentralAbility)
  {
    CentralAbility->ResolveAbility();
  }

  UpdateExhaust();
}

void AInteractiveCharacter::UpdateExhaust()
{
  if (CharacterDataCore.bIsExhausted)
  {
    return;
  }

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
  else
  {
    PlayAnimation(IDLE_ANIMATION_ID, false);
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

  TTuple<UPaperFlipbook*, float, int32> FoundAnimationData{ nullptr, 0, 0 };
  while (CharacterDataCore.AnimationQueue.Dequeue(FoundAnimationData)) 
  {
    if (FoundAnimationData.Get<2>() == EXHAUST_ANIMATION_ID)
    {
      while (CharacterDataCore.AnimationQueue.Pop()) {};
      CharacterPresentation->SetLooping(false);
      OnExhausted();
      break;
    }
  }

  if (FoundAnimationData.Get<0>())
  {
    CharacterPresentation->SetFlipbook(FoundAnimationData.Get<0>());
    ActionWithAnimation(FoundAnimationData.Get<1>(), FoundAnimationData.Get<2>());
  }
}

void AInteractiveCharacter::ActionWithAnimation_Implementation(float Duration, int32 AnimationID)
{

}

void AInteractiveCharacter::PrepareCentralAbilityToResolve()
{
  if (CentralAbility)
  {
    CentralAbility->PrepareToResolve();
  }
}

AInteractiveAbility* AInteractiveCharacter::GetCentralAbility() const
{
  return CentralAbility;
}

TArray<AInteractiveAbility*>* AInteractiveCharacter::GetAbilties()
{
  return &Abilities;
}

void AInteractiveCharacter::PrepareToResolve_Implementation()
{
  if (!CentralAbility) return;

  for (AInteractiveObject* Target : CentralAbility->GetInfluences())
  {
    TargetLocations.Add(Target->GetActorLocation());
  }
}

void AInteractiveCharacter::AfterResolution_Implementation()
{
  TargetLocations.Empty();
}

void AInteractiveCharacter::PopTarget()
{
  LastStartLocation = GetActorLocation();

  if (TargetLocations.Num())
  {
    LastTargetLocation = TargetLocations.Pop();
  }
  else
  {
    LastTargetLocation = GetActorLocation();
  }
}
