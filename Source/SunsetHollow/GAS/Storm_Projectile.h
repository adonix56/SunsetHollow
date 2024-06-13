// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "GameplayEffect.h"
#include "Storm_Projectile.generated.h"

class UCapsuleComponent;
class UGameplayEffect;
class UAbilitySystemComponent;
class UParticleSystemComponent;
class UProjectileMovementComponent;

UCLASS()
class SUNSETHOLLOW_API AStorm_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStorm_Projectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float Timer;
	float TimeSinceLastDamage;
	FGameplayEffectSpecHandle DamageSpec;
	UAbilitySystemComponent* PlayerAbilitySystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float MaxScale = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float GrowthSpeed = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float Lifespan = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float DamageInterval = 0.2f;

	USceneComponent* SceneRootComponent;

	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* Storm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> GE_DealDamage;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
