// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "QuickCastAbility.generated.h"

class AEnemyCharacter;
/**
 * 
 */
UCLASS()
class SUNSETHOLLOW_API UQuickCastAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	FRotator GetRotatorDirectionFromVector(FVector DirectionVector);

	UFUNCTION(BlueprintCallable)
	FVector GetRelativeLocationWithOffset(AActor* Actor, bool bWithOffset, FVector Offset);
	
protected:
	UFUNCTION(BlueprintCallable)
	void TurnCharacterTowardsCursor(FVector& OutLocation, FVector& OutDirection);

	UFUNCTION(BlueprintCallable)
	void DamageEnemy(AActor* TargetEnemy, EDamageAppliedType DamageType, float Damage, float MoveDistance = 0.0f, FVector Direction = FVector::ZeroVector, bool FacePlayer = true);

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Damage, meta = (AllowPrivateAccess = "true"))
	//float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Damage, meta = (AllowPrivateAccess = "true"))
	TSet<AEnemyCharacter*> HitEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Advanced, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> GEDealDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Advanced, meta = (AllowPrivateAccess = "true"))
	FGameplayTag DamageTag;
};
