// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "Templates\SubclassOf.h"
#include "SunsetHollowCharacter.h"
#include "Interactable.h"
#include "SunsetHollowPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

USTRUCT(BlueprintType)
struct FSunsetHollowGameplayAbilityInput {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UGameplayAbility> GameplayAbility;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* InputAction;
};

UENUM(BlueprintType)
enum class EUsableAbility : uint8 {
	DASH = 0 UMETA(DisplayName = "Dash"),
	GUILLOTINE = 1 UMETA(DisplayName = "Guillotine"),
	STORM = 2 UMETA(DisplayName = "Storm"),
	SPINSAW = 3 UMETA(DisplayName = "Spin Saw"),
	SUPERNOVA = 4 UMETA(DisplayName = "Super Nova")
};

ENUM_RANGE_BY_FIRST_AND_LAST(EUsableAbility, EUsableAbility::DASH, EUsableAbility::SUPERNOVA);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityCooldown, EUsableAbility, AbilityOnCooldown, float, CooldownDuration);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSwapMouse, bool, LeftClickMove);

UCLASS()
class ASunsetHollowPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASunsetHollowPlayerController();

	void TickActor(float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GAS, meta = (AllowPrivateAccess = "true"))
	TMap<EUsableAbility, float> CooldownManager;

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* LeftClick;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* RightClick;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwapMouse;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Interact;

	/** Jump Input Action */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	//UInputAction* SetDestinationTouchAction;

	void StartDie();

	UFUNCTION(BlueprintCallable)
	void StartRespawn();

	UPROPERTY(BlueprintAssignable)
	FOnAbilityCooldown OnAbilityCooldownEvent;
	UPROPERTY(BlueprintReadWrite)
	FOnSwapMouse OnSwapMouseEvent;

	UFUNCTION(BlueprintCallable)
	void SetInteractable(AInteractable* NewInteractable);

	UFUNCTION(BlueprintCallable)
	void RemoveInteractable(AInteractable* Interactable);

	UFUNCTION(BlueprintCallable)
	bool HasInteractable() { return CurrentInteractable != nullptr; }

	UFUNCTION(BlueprintCallable)
	void SetControllableState(bool CanControl);

	UFUNCTION(BlueprintCallable)
	float GetAbilityCooldown(EUsableAbility Ability) { return CooldownManager[Ability]; }

	UFUNCTION(BlueprintCallable)
	void ResetAllAbilityCooldowns();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	bool bLeftClickMove = true;

	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GAS)
	TArray<FSunsetHollowGameplayAbilityInput> GameplayAbilityArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	AInteractable* CurrentInteractable;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GAS, meta = (AllowPrivateAccess = "true"))
	//UAbilitySystemComponent* GASComponent;

	//virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return GASComponent; }
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void OnPossess(APawn* aPawn) override;

	void OnLeftClickStarted();
	void OnLeftClickTriggered();
	void OnLeftClickReleased();
	
	void OnRightClickStarted();
	void OnRightClickTriggered();
	void OnRightClickReleased();

	void ActivateAbilityByIndex(int AbilityIndex, bool SetIsAttacking = false);

	void OnSpacebarStarted();
	void OnInteract();
	bool bControllable;
	void EndInteract();
	bool IsControllable();

	void OnQStarted();
	void OnWStarted();
	void OnEStarted();
	void OnRStarted();

	void OnSwapMouse();

	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	void OnTouchTriggered();
	void OnTouchReleased();
	void OnBasicAttack();

	UFUNCTION()
	void OnCooldownCheck(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle GEHandle);
	UFUNCTION()
	void StartCooldown(EUsableAbility Ability, float CooldownDuration);

	UPROPERTY(Transient)
	UUserWidget* DeathUI;
	ASunsetHollowGameState* GameState;

private:
	FVector CachedDestination;

	//UPROPERTY(VisibleAnywhere, Transient)
	//ASunsetHollowCharacter* SunsetCharacter;

	ASunsetHollowCharacter* GetSunsetCharacter() { return Cast<ASunsetHollowCharacter>(GetPawn()); }

	//TWeakObjectPtr<ASunsetHollowCharacter> SunsetCharacterTwo;

	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> DeathUIClass;
};


