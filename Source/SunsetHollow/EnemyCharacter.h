// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GAS/SunsetHollowBaseAttributeSet.h"
#include "EnemyCharacter.generated.h"

class UBehaviorTree;
class UAnimMontage;
class UEnemyAbility;

UENUM(BlueprintType)
enum class EDamageAppliedType : uint8 {
	BASIC = 0 UMETA(DisplayName = "Basic"),
	KNOCKUP = 1 UMETA(DisplayName = "Knockup"),
	KNOCKBACK = 2 UMETA(DisplayName = "Knockback")
};

UCLASS()
class SUNSETHOLLOW_API AEnemyCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	UAbilitySystemComponent* GetAbilitySystemComponent() const override { return GASComponent; }

	UBehaviorTree* GetBehaviorTree() const { return Tree; }

	float GetSightRadius() const { return SightRadius; }
	
	float GetLoseSightRadius() const { return SightRadius + LoseSightDistance; }

	UFUNCTION(BlueprintImplementableEvent)
	void BP_HandleDamageAnimation(EDamageAppliedType DamageType);

	void HandleDamageAnimation(EDamageAppliedType DamageType, float DistanceMoved = 0.0f, FVector LaunchDirection = FVector::ZeroVector, bool FacePlayer = true);

	UFUNCTION(BlueprintCallable)
	bool IsInDamageAnim();

	void StartDie();

	UFUNCTION(BlueprintCallable)
	void EndDie();

	bool IsDying() { return Dying; }
	
	UFUNCTION(BlueprintCallable)
	bool CastRandomAbility();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GAS, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UEnemyAbility>> AvailableAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GAS, meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent* GASComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GAS, meta = (AllowPrivateAccess = "true"))
	const USunsetHollowBaseAttributeSet* BaseAttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* Tree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	float SightRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	float LoseSightDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Damage, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* BasicAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Damage, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* KnockupAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Damage, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* KnockbackAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Damage, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DieAnim;

	UAnimMontage* PlayAnim;
	FDelegateHandle HealthChangedDelegateHandle;
	FGameplayTagContainer DamagedTagContainer;

	void HealthChanged(const FOnAttributeChangeData& Data);
	bool Dying = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
