// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAbility.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "../SunsetHollowCharacter.h"

FRotator UEnemyAbility::GetRotatorDirectionFromVector(FVector DirectionVector)
{
	float Yaw = UKismetMathLibrary::DegAtan(UKismetMathLibrary::SafeDivide(DirectionVector.Y, DirectionVector.X));
	Yaw += DirectionVector.X <= 0 ? 180 : 0;
	return FRotator(0, Yaw, 0);
}

FVector UEnemyAbility::GetRelativeLocationWithOffset(AActor* Actor, bool bWithOffset, FVector Offset)
{
	FVector TargetLocation = Actor->GetActorLocation();

	if (bWithOffset) {
		TargetLocation += Actor->GetActorForwardVector() * Offset.X;
		TargetLocation += Actor->GetActorRightVector() * Offset.Y;
		TargetLocation += Actor->GetActorUpVector() * Offset.Z;
	}

	return FVector(TargetLocation);
}

void UEnemyAbility::TurnTowardsActor(AActor* LookAtActor)
{
	AActor* OwningActor = GetOwningActorFromActorInfo();
	FVector Start = OwningActor->GetActorLocation();
	FVector End = LookAtActor->GetActorLocation();
	Start.Z = 0.0f;
	End.Z = 0.0f;
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);
	OwningActor->SetActorRotation(LookAtRotation);
}

void UEnemyAbility::DamagePlayer(AActor* TargetPlayer, DamageAppliedType DamageType, float DamageDealt, float MoveDistance, FVector Direction, bool FaceMe)
{
	if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(GetOwningActorFromActorInfo())) {
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(GEDealDamage);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageTag, DamageDealt);
		if (ASunsetHollowCharacter* PlayerCharacter = Cast<ASunsetHollowCharacter>(TargetPlayer)) {
			if (!HitPlayers.Contains(PlayerCharacter)) {
				HitPlayers.Add(PlayerCharacter);
				Enemy->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), PlayerCharacter->GetAbilitySystemComponent());

				//TODO: Player handle damage animation
			}
		}
	}
}
