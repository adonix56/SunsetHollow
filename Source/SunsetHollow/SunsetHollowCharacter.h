// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GAS/SunsetHollowBaseAttributeSet.h"
#include "SunsetHollowCharacter.generated.h"

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

protected:
	virtual void BeginPlay();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GAS, meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent* GASComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GAS, meta = (AllowPrivateAccess = "true"))
	const USunsetHollowBaseAttributeSet* BaseAttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GAS, meta = (AllowPrivateAccess = "true"))
	int AttackCount = 0;

	bool bZoom;
	bool bReverse;
	float ElapsedTime;
	float ZoomTime;
	float TargetDistance;
	float Delay;

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	class UAIPerceptionStimuliSourceComponent* StimulusSource;

	void SetupStimulusSource();
};

