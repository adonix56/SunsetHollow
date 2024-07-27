// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GAS/SunsetHollowBaseAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SunsetHollowGameState.h"
#include "SunsetHollowCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FCharacterDeath, ASunsetHollowCharacter*)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChange, float, NewHealthValue);

class UAbilitySystemComponent;

UCLASS(Blueprintable)
class ASunsetHollowCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASunsetHollowCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return GASComponent; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GAS, meta = (AllowPrivateAccess = "true"))
	bool bIsAttacking;

	int GetAttackCount() { return AttackCount; }
	void IncreaseAttackCount();
	void ResetAttackCount() { AttackCount = 0; }

	UFUNCTION(BlueprintCallable)
	void IgnoreEnemyCollision(bool IgnoreCollision);

	UFUNCTION(BlueprintCallable)
	void ZoomCameraWithTimer(float zoomTime, float targetDistance, float speed, float delay);

	void StartDie();
	void StartRespawn();
	void EndRespawn();

	UFUNCTION(BlueprintCallable)
	bool IsDying() const { return bIsDead; }
	UFUNCTION(BlueprintCallable)
	bool IsRespawning() const { return bIsRespawning; }
	FCharacterDeath CharacterDeathEvent;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChange HealthChangeEvent;

	UFUNCTION(BlueprintCallable)
	void JumpToDestination(FVector Destination);

	UFUNCTION(BlueprintCallable)
	float GetPlayerDistance() { return DistanceMoved; }

protected:
	virtual void BeginPlay();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GAS, meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent* GASComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GAS, meta = (AllowPrivateAccess = "true"))
	const USunsetHollowBaseAttributeSet* BaseAttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GAS, meta = (AllowPrivateAccess = "true"))
	int AttackCount = 0;

	bool bIsDead;
	bool bIsRespawning;
	bool bZoom;
	bool bReverse;
	float ElapsedTime;
	float ZoomTime;
	float TargetDistance;
	float Delay;
	float DistanceMoved;
	ASunsetHollowGameState* GameState;
	FVector LastLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GAS, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* SpinsawCurve;

	void HealthChanged(const FOnAttributeChangeData& Data);

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Damage, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DieAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Perception, meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionStimuliSourceComponent* StimulusSource;

	void SetupStimulusSource();
};

