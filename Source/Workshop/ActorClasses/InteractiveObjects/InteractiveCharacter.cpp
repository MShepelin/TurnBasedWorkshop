// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveCharacter.h"
#include "Workshop/UI/TurnBasedEvent/InformationWidget.h"
#include "Workshop/UI/TurnBasedEvent/AbilitiesWidget.h"
#include "Components/BillboardComponent.h"
#include "Workshop/UI/AbilitySlot.h"
#include "Kismet/GameplayStatics.h"
#include "InteractiveAbility.h"
#include "Workshop/UI/TurnBasedEvent/TurnBasedHUD.h"
#include "Workshop/GameControl/PlayerControllers/CameraController.h"
#include "InteractiveAbility.h"


AInteractiveCharacter::AInteractiveCharacter()
{
  InteractiveDataCore.InteractiveType = static_cast<int32>(EInteractiveType::Character);

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
}

void AInteractiveCharacter::PlayAnimation(int32 AnimationId)
{
  UPaperFlipbook** FoundFlipbook = CharacterDataCore.AnimationsMap.Find(AnimationId);

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

  if (!CharacterDataCore.AnimationsMap.Find(IdleAnimation))
  {
    UE_LOG(LogTemp, Error, TEXT("%d animation id (IdleAnimation) must be set"), IdleAnimation);
  }

#ifdef WITH_EDITOR
  check(CharacterDataCore.CharacterStats.Num() == CharacterIntegerStats);

  //Check if any of already used stat IDs are met.
  for (TTuple<int32, FBar>& NecessaryStat : InteractiveDataCore.IntegerStats)
  {
    if (NecessaryStat.Get<0>() >= 0 && NecessaryStat.Get<0>() < CharacterIntegerStats)
    {
      UE_LOG(LogTemp, Error, TEXT("Stat IDs from 0 to %d are allocated and can't be used!"), CharacterIntegerStats - 1);
    }
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

  CentralAbilityRelativePosition = FVector(
    CentralAbilityRelativePositionInput[0], WidgetComponentYOrder, CentralAbilityRelativePositionInput[1]);

  RefreshInteractive();
}

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
}

void AInteractiveCharacter::SetCentralAbility(AInteractiveAbility* Ability) 
{
  ClearCentralAbility();

  CentralAbility = Ability;
  CentralAbility->SetActorLocation(CentralAbilityRelativePosition + CollisionBox->GetComponentLocation());
  ChangeCentralAbilityVisibility(false);
}

void AInteractiveCharacter::ChangeCentralAbilityVisibility(bool bIsInvisible) //++++ change function name
{
  check(CentralAbility != nullptr);
  CentralAbility->SetActorHiddenInGame(bIsInvisible);
}

void AInteractiveCharacter::RefreshInteractive()
{
  // ----------------- //
  // Refresh animation //
  // ----------------- //
  if (!CharacterDataCore.AnimationsMap.Find(IdleAnimation))
  {
    return;
  }

  CharacterPresentation->SetFlipbook(CharacterDataCore.AnimationsMap[IdleAnimation]);

  FVector ScaledBoxExtent(CollisionBox->GetUnscaledBoxExtent());
  CollisionBox->SetBoxExtent(FVector(ScaledBoxExtent[0], CollisionBoxWidth, ScaledBoxExtent[2]));

  // Set y-order
  CentralAbilityPositionVisual->SetRelativeLocation(CentralAbilityRelativePosition);

  FVector PresentationLocation = CharacterPresentation->GetRelativeLocation();
  CharacterPresentation->SetRelativeLocation(FVector(
    PresentationLocation[0], MainSpriteYOrder, PresentationLocation[2]));

  FVector BoxLocation = CollisionBox->GetRelativeLocation();
  CollisionBox->SetRelativeLocation(FVector(
    BoxLocation[0], MainSpriteYOrder, BoxLocation[2]));

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
    //AbilityObject->SetActorHiddenInGame(true);
    Abilities.Add(AbilityObject);
  }
}

void AInteractiveCharacter::ClearCentralAbility()
{
  if (!CentralAbility)
  {
    return;
  }

  if (CentralAbility->IsCentralInManager()) //++++ sounds ambiguous, change termins!
  {
    CentralAbility->UnpickedAsCentral();
  }

  CentralAbility->ClearDependencies();
  CentralAbility->ClearInflunces();
  ChangeCentralAbilityVisibility(false);

  ACameraController* CurrentController = Cast<ACameraController>(UGameplayStatics::GetPlayerController(this, 0));
  FVector HiddenLocation = CurrentController->GetCurrentCamera()->GetHiddenLocation(); //???? may be save hidden location somewhere
  CentralAbility->SetActorLocation(HiddenLocation);
}

void AInteractiveCharacter::ResolveCharacterActions()
{
  if (!CentralAbility)
  {
    return;
  }

  CentralAbility->ResolveAbility();
}

void AInteractiveCharacter::UpdateCharacterStatus()
{
  size_t StatIndex = 0;
  for (uint8 StatusMask = 1; StatIndex < 8; StatIndex++)
  {
    if (CharacterDataCore.CharacterStats[StatIndex].IsActive())
    {
      CharacterDataCore.CharacterStatus |= StatusMask;
    }
    else
    {
      CharacterDataCore.CharacterStatus &= ~StatusMask;
    }

    StatusMask = StatusMask << 1;
  }
}
