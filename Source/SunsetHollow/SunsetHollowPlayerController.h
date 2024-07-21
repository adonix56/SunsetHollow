// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
//#include "AbilitySystemInterface.h"
#include "Templates\SubclassOf.h"
#include "SunsetHollowCharacter.h"
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
enum class FUsableAbility : uint8 {
	DASH = 0 UMETA(DisplayName = "Dash"),
	GUILLOTINE = 1 UMETA(DisplayName = "Guillotine"),
	STORM = 2 UMETA(DisplayName = "Storm"),
	SPINSAW = 3 UMETA(DisplayName = "Spin Saw"),
	SUPERNOVA = 4 UMETA(DisplayName = "Super Nova")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityCooldown, FUsableAbility, AbilityOnCooldown, float, CooldownDuration);

UCLASS()
class ASunsetHollowPlayerController : public APlayerController//, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASunsetHollowPlayerController();

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SwapMouse;

	/** Jump Input Action */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	//UInputAction* SetDestinationTouchAction;

	void StartDie();

	UPROPERTY(BlueprintAssignable)
	FOnAbilityCooldown OnAbilityCooldownEvent;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	bool bLeftClickMove = true;

	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GAS)
	TArray<FSunsetHollowGameplayAbilityInput> GameplayAbilityArray;

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

private:
	FVector CachedDestination;

	//UPROPERTY(VisibleAnywhere, Transient)
	//ASunsetHollowCharacter* SunsetCharacter;

	ASunsetHollowCharacter* GetSunsetCharacter() { return Cast<ASunsetHollowCharacter>(GetPawn()); }

	//TWeakObjectPtr<ASunsetHollowCharacter> SunsetCharacterTwo;

	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed
};


