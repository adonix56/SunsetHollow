// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickCastAbility.h"
#include "../EnemyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "../SunsetHollowCharacter.h"
#include "GameplayEffect.h"

FRotator UQuickCastAbility::GetRotatorDirectionFromVector(FVector DirectionVector)
{
	float Yaw = UKismetMathLibrary::DegAtan(UKismetMathLibrary::SafeDivide(DirectionVector.Y, DirectionVector.X));
	Yaw += DirectionVector.X <= 0 ? 180 : 0;
	return FRotator(0, Yaw, 0);
}

FVector UQuickCastAbility::GetRelativeLocationWithOffset(AActor* Actor, bool bWithOffset, FVector Offset)
{
	FVector TargetLocation = Actor->GetActorLocation();

	if (bWithOffset) {
		TargetLocation += Actor->GetActorForwardVector() * Offset.X;
		TargetLocation += Actor->GetActorRightVector() * Offset.Y;
		TargetLocation += Actor->GetActorUpVector() * Offset.Z;
	}

	return FVector(TargetLocation);
}

void UQuickCastAbility::TurnCharacterTowardsCursor(FVector& OutLocation, FVector& OutDirection) {
	FVector WorldLocation;
	FVector WorldDirection;
	if (GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(WorldLocation, WorldDirection)) {
		FHitResult OnHit;
		WorldDirection = (WorldDirection * 5000) + WorldLocation;
		if (GetWorld()->LineTraceSingleByChannel(OnHit, WorldLocation, WorldDirection, ECollisionChannel::ECC_Visibility)) {
			AActor* OwningActor = GetOwningActorFromActorInfo();
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(OwningActor->GetActorLocation(), OnHit.Location);
			OwningActor->SetActorRotation(LookAtRotation);

			OutLocation = OnHit.Location;
			OutDirection = OnHit.Location - OwningActor->GetActorLocation();
		}
	}
}

void UQuickCastAbility::DamageEnemy(AActor* TargetEnemy, DamageAppliedType DamageType, float MoveDistance, FVector Direction)
{
	if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(TargetEnemy)) {
		if (!HitEnemies.Contains(Enemy)) {
			HitEnemies.Add(Enemy);
			FGameplayEffectSpecHandle SpecHandle =  MakeOutgoingGameplayEffectSpec(GEDealDamage);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageTag, Damage);
			if (ASunsetHollowCharacter* PlayerCharacter = Cast<ASunsetHollowCharacter>(GetOwningActorFromActorInfo())) {
				PlayerCharacter->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), Enemy->GetAbilitySystemComponent());
				Enemy->HandleDamageAnimation(DamageType, MoveDistance, Direction);
				UE_LOG(LogTemp, Warning, TEXT("Damaged Enemy!"));
			}
		}
	}
}
