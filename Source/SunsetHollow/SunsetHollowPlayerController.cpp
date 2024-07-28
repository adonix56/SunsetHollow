// Copyright Epic Games, Inc. All Rights Reserved.

#include "SunsetHollowPlayerController.h"
#include "SunsetHollowGameMode.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "SunsetHollowCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Blueprint/UserWidget.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ASunsetHollowPlayerController::ASunsetHollowPlayerController()
{
	bControllable = true;
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;

	for (EUsableAbility Ability : TEnumRange<EUsableAbility>()) {
		CooldownManager.Add(Ability, -1.0f);
	}
	OnAbilityCooldownEvent.AddDynamic(this, &ASunsetHollowPlayerController::StartCooldown);
}

void ASunsetHollowPlayerController::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);
	for (EUsableAbility Ability : TEnumRange<EUsableAbility>()) {
		if (CooldownManager[Ability] > 0.0f) CooldownManager[Ability] -= DeltaTime;
	}
}

void ASunsetHollowPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
		GameState = Cast<ASunsetHollowGameState>(GetWorld()->GetGameState());
	}
}

void ASunsetHollowPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	ASunsetHollowCharacter* SunsetCharacter = GetSunsetCharacter();
	if (SunsetCharacter) {
		for (FSunsetHollowGameplayAbilityInput GASInput : GameplayAbilityArray) {
			SunsetCharacter->GetAbilitySystemComponent()->GiveAbility(FGameplayAbilitySpec(GASInput.GameplayAbility));
		}
		SunsetCharacter->GetAbilitySystemComponent()->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &ASunsetHollowPlayerController::OnCooldownCheck);
	}
}

void ASunsetHollowPlayerController::StartDie()
{
	if (ASunsetHollowCharacter* SunsetCharacter = GetSunsetCharacter()) {
		SunsetCharacter->StartDie();
		StopMovement();
		if (DeathUI == nullptr) {
			DeathUI = CreateWidget<UUserWidget>(this, DeathUIClass);
			DeathUI->AddToViewport(9999);
		}
	}
}

void ASunsetHollowPlayerController::StartRespawn()
{
	if (ASunsetHollowGameMode* GameMode = Cast<ASunsetHollowGameMode>(GetWorld()->GetAuthGameMode())) {
		if (ASunsetHollowCharacter* SunsetCharacter = GetSunsetCharacter()) {
			GameMode->RespawnPlayer(SunsetCharacter);
			SunsetCharacter->StartRespawn();
			DeathUI->RemoveFromParent();
			DeathUI = nullptr; // UE Garbage Collection will auto destroy unreferenced widgets
		}
	}
}

void ASunsetHollowPlayerController::SetInteractable(AInteractable* NewInteractable)
{
	CurrentInteractable = NewInteractable;
}

void ASunsetHollowPlayerController::RemoveInteractable(AInteractable* Interactable)
{
	if (CurrentInteractable == Interactable) {
		CurrentInteractable = nullptr;
	}
}

void ASunsetHollowPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(LeftClick, ETriggerEvent::Started, this, &ASunsetHollowPlayerController::OnLeftClickStarted);
		EnhancedInputComponent->BindAction(LeftClick, ETriggerEvent::Triggered, this, &ASunsetHollowPlayerController::OnLeftClickTriggered);
		EnhancedInputComponent->BindAction(LeftClick, ETriggerEvent::Completed, this, &ASunsetHollowPlayerController::OnLeftClickReleased);
		EnhancedInputComponent->BindAction(LeftClick, ETriggerEvent::Canceled, this, &ASunsetHollowPlayerController::OnLeftClickReleased);

		EnhancedInputComponent->BindAction(RightClick, ETriggerEvent::Started, this, &ASunsetHollowPlayerController::OnRightClickStarted);
		EnhancedInputComponent->BindAction(RightClick, ETriggerEvent::Triggered, this, &ASunsetHollowPlayerController::OnRightClickTriggered);
		EnhancedInputComponent->BindAction(RightClick, ETriggerEvent::Completed, this, &ASunsetHollowPlayerController::OnRightClickReleased);
		EnhancedInputComponent->BindAction(RightClick, ETriggerEvent::Canceled, this, &ASunsetHollowPlayerController::OnRightClickReleased);

		EnhancedInputComponent->BindAction(SwapMouse, ETriggerEvent::Started, this, &ASunsetHollowPlayerController::OnSwapMouse);

		EnhancedInputComponent->BindAction(Interact, ETriggerEvent::Started, this, &ASunsetHollowPlayerController::OnInteract);

		EnhancedInputComponent->BindAction(GameplayAbilityArray[0].InputAction, ETriggerEvent::Started, this, &ASunsetHollowPlayerController::OnSpacebarStarted);

		EnhancedInputComponent->BindAction(GameplayAbilityArray[4].InputAction, ETriggerEvent::Started, this, &ASunsetHollowPlayerController::OnQStarted);
		EnhancedInputComponent->BindAction(GameplayAbilityArray[5].InputAction, ETriggerEvent::Started, this, &ASunsetHollowPlayerController::OnWStarted);
		EnhancedInputComponent->BindAction(GameplayAbilityArray[6].InputAction, ETriggerEvent::Started, this, &ASunsetHollowPlayerController::OnEStarted);
		EnhancedInputComponent->BindAction(GameplayAbilityArray[7].InputAction, ETriggerEvent::Started, this, &ASunsetHollowPlayerController::OnRStarted);

		// Setup touch input events
		//EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &ASunsetHollowPlayerController::OnInputStarted);
		//EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &ASunsetHollowPlayerController::OnTouchTriggered);
		//EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &ASunsetHollowPlayerController::OnTouchReleased);
		//EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &ASunsetHollowPlayerController::OnTouchReleased);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ASunsetHollowPlayerController::OnLeftClickStarted()
{
	if (bLeftClickMove) { OnInputStarted(); }
	else { OnBasicAttack(); }//BPF_Attack(); }
}

void ASunsetHollowPlayerController::OnLeftClickTriggered()
{
	if (bLeftClickMove) { OnSetDestinationTriggered(); }
}

void ASunsetHollowPlayerController::OnLeftClickReleased()
{
	if (bLeftClickMove) { OnSetDestinationReleased(); }
}

void ASunsetHollowPlayerController::OnRightClickStarted()
{
	if (!bLeftClickMove) { OnInputStarted(); }
	else { OnBasicAttack(); }//BPF_Attack(); }
}

void ASunsetHollowPlayerController::OnRightClickTriggered()
{
	if (!bLeftClickMove) { OnSetDestinationTriggered(); }
}

void ASunsetHollowPlayerController::OnRightClickReleased()
{
	if (!bLeftClickMove) { OnSetDestinationReleased(); }
}

void ASunsetHollowPlayerController::ActivateAbilityByIndex(int AbilityIndex, bool SetIsAttacking)
{
	if (!IsControllable()) return;
	if (AbilityIndex >= 0 && AbilityIndex < GameplayAbilityArray.Num()) {
		ASunsetHollowCharacter* SunsetCharacter = GetSunsetCharacter();
		if (SunsetCharacter) {
			if (!SetIsAttacking || (SetIsAttacking && !SunsetCharacter->bIsAttacking)) {
				UAbilitySystemComponent* GASComponent = SunsetCharacter->GetAbilitySystemComponent();
				//GASComponent->CancelAllAbilities();
				if (GASComponent->TryActivateAbilityByClass(GameplayAbilityArray[AbilityIndex].GameplayAbility)) {
					StopMovement();
					SunsetCharacter->bIsAttacking = SetIsAttacking;
					SunsetCharacter->ResetAttackCount();
				}
			}
		}
	}
}

void ASunsetHollowPlayerController::OnSpacebarStarted() {
	if (GameState && GameState->GetCurrentGameState() >= EGameState::TUTORIAL_DASH) 
		ActivateAbilityByIndex(0);
}

void ASunsetHollowPlayerController::OnInteract()
{
	if (!IsControllable()) return;
	if (CurrentInteractable) {
		SetControllableState(false);
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ASunsetHollowPlayerController::EndInteract, 0.1f, false); // Prevents Movement on next frame.
		CurrentInteractable->Interact();
	}
}

void ASunsetHollowPlayerController::EndInteract()
{
	SetControllableState(true);
}

void ASunsetHollowPlayerController::SetControllableState(bool CanControl)
{
	if (!CanControl) StopMovement();
	bControllable = CanControl;
}

bool ASunsetHollowPlayerController::IsControllable()
{
	ASunsetHollowCharacter* PlayerCharacter = GetSunsetCharacter();
	if (!bControllable) return false;
	if (PlayerCharacter) {
		if (PlayerCharacter->IsDying()) return false;
		if (PlayerCharacter->GetMovementComponent()->IsFalling()) return false;
	}
	return true;
}

void ASunsetHollowPlayerController::OnQStarted()
{
	if (GameState && GameState->GetCurrentGameState() >= EGameState::TUTORIAL_SKILLS)
		ActivateAbilityByIndex(4, true);
}

void ASunsetHollowPlayerController::OnWStarted()
{
	if (GameState && GameState->GetCurrentGameState() >= EGameState::TUTORIAL_SKILLS)
		ActivateAbilityByIndex(5, true);
}

void ASunsetHollowPlayerController::OnEStarted()
{
	if (GameState && GameState->GetCurrentGameState() >= EGameState::TUTORIAL_SKILLS)
		ActivateAbilityByIndex(6, true);
}

void ASunsetHollowPlayerController::OnRStarted()
{
	if (GameState && GameState->GetCurrentGameState() >= EGameState::TUTORIAL_SKILLS)
		ActivateAbilityByIndex(7, true);
}

void ASunsetHollowPlayerController::OnSwapMouse() {
	bLeftClickMove = !bLeftClickMove;
	OnSwapMouseEvent.ExecuteIfBound(bLeftClickMove);
}

void ASunsetHollowPlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void ASunsetHollowPlayerController::OnSetDestinationTriggered()
{
	if (!IsControllable()) return;
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}
	
	/*/ Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}*/
	// If it was a short press
	if (!GetSunsetCharacter()->bIsAttacking) UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
}

void ASunsetHollowPlayerController::OnSetDestinationReleased()
{
	if (!IsControllable()) return;
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		if (!GetSunsetCharacter()->bIsAttacking) UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}
	FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void ASunsetHollowPlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void ASunsetHollowPlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}

void ASunsetHollowPlayerController::OnBasicAttack() {
	if (GameState && GameState->GetCurrentGameState() >= EGameState::TUTORIAL_AA) {
		ASunsetHollowCharacter* SunsetCharacter = GetSunsetCharacter();
		if (!IsControllable()) return;

		if (SunsetCharacter && !SunsetCharacter->bIsAttacking) {
			StopMovement();
			int AbilityToActivate = SunsetCharacter->GetAttackCount() + 1;
			if (SunsetCharacter->GetAbilitySystemComponent()->TryActivateAbilityByClass(GameplayAbilityArray[AbilityToActivate].GameplayAbility)) {
				SunsetCharacter->IncreaseAttackCount();
				SunsetCharacter->bIsAttacking = true;
			}
		}
	}
}

void ASunsetHollowPlayerController::OnCooldownCheck(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle GEHandle)
{
	FGameplayTagContainer GrantedTags;
	GESpec.GetAllGrantedTags(GrantedTags);
	FString(TagString) = GrantedTags.GetByIndex(0).GetTagName().ToString();
	if (TagString.Contains("Cooldown")) {
		if (TagString.Contains("Dash")) OnAbilityCooldownEvent.Broadcast(EUsableAbility::DASH, GESpec.Duration);
		else if (TagString.Contains("Guillotine")) OnAbilityCooldownEvent.Broadcast(EUsableAbility::GUILLOTINE, GESpec.Duration);
		else if (TagString.Contains("Storm")) OnAbilityCooldownEvent.Broadcast(EUsableAbility::STORM, GESpec.Duration);
		else if (TagString.Contains("SpinSaw")) OnAbilityCooldownEvent.Broadcast(EUsableAbility::SPINSAW, GESpec.Duration);
		else if (TagString.Contains("SuperNova")) OnAbilityCooldownEvent.Broadcast(EUsableAbility::SUPERNOVA, GESpec.Duration);
		UE_LOG(LogTemp, Warning, TEXT("OnCooldown!! %s"), *GrantedTags.GetByIndex(0).GetTagName().ToString());
	}
}

void ASunsetHollowPlayerController::StartCooldown(EUsableAbility Ability, float CooldownDuration)
{
	CooldownManager[Ability] = CooldownDuration;
}
