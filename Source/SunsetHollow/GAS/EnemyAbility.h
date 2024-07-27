// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "../EnemyCharacter.h"
#include "../SunsetHollowCharacter.h"
#include "EnemyAbility.generated.h"

/**
 * 
 */
UCLASS()
class SUNSETHOLLOW_API UEnemyAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	FRotator GetRotatorDirectionFromVector(FVector DirectionVector);

	UFUNCTION(BlueprintCallable)
	FVector GetRelativeLocationWithOffset(AActor* Actor, bool bWithOffset, FVector Offset);

protected:
	UFUNCTION(BlueprintCallable)
	void TurnTowardsActor(AActor* LookAtActor);

	UFUNCTION(BlueprintCallable)
	void DamagePlayer(AActor* TargetPlayer, EDamageAppliedType DamageType, float DamageDealt, float MoveDistance = 0.0f, FVector Direction = FVector::ZeroVector, bool FaceMe = true);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Advanced, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> GEDealDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Advanced, meta = (AllowPrivateAccess = "true"))
	FGameplayTag DamageTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Damage, meta = (AllowPrivateAccess = "true"))
	TSet<ASunsetHollowCharacter*> HitPlayers;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Damage, meta = (AllowPrivateAccess = "true"))
	float Damage;
};
