// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickCastAbility.h"
#include "../EnemyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "../SunsetHollowCharacter.h"
#include "GameplayEffect.h"

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

void UQuickCastAbility::DamageEnemy(AActor* TargetEnemy)
{
	if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(TargetEnemy)) {
		if (!HitEnemies.Contains(Enemy)) {
			HitEnemies.Add(Enemy);
			FGameplayEffectSpecHandle SpecHandle =  MakeOutgoingGameplayEffectSpec(GEDealDamage);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageTag, Damage * -1);
			if (ASunsetHollowCharacter* PlayerCharacter = Cast<ASunsetHollowCharacter>(GetOwningActorFromActorInfo())) {
				PlayerCharacter->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), Enemy->GetAbilitySystemComponent());
				UE_LOG(LogTemp, Warning, TEXT("Damaged Enemy!"));
			}
		}
	}
}
