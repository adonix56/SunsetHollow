// Fill out your copyright notice in the Description page of Project Settings.


#include "SunsetHollowBaseAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "../EnemyCharacter.h"
#include "../EnemyAIController.h"
#include "../SunsetHollowCharacter.h"
#include "../SunsetHollowPlayerController.h"

void USunsetHollowBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute()) {
		const float DamageDealt = GetDamage();
		SetDamage(0.f);
		
		if (DamageDealt > 0.f) {
			AActor* TargetActor = Data.Target.GetOwner();
			if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(TargetActor)) { // Enemy Taking Damage
				if (AEnemyAIController* EnemyCont = Cast<AEnemyAIController>(Enemy->GetController())) {
					SetHealth(FMath::Clamp(GetHealth() - DamageDealt, 0.0f, GetMaxHealth()));
					if (GetHealth() < 0.03f) {
						EnemyCont->StartDie();
					}
				}
			}

			if (ASunsetHollowCharacter* Player = Cast<ASunsetHollowCharacter>(TargetActor)) { // Player Taking Damage
				if (ASunsetHollowPlayerController* PlayerCont = Cast<ASunsetHollowPlayerController>(Player->GetController())) {
					SetHealth(FMath::Clamp(GetHealth() - DamageDealt, 0.0f, GetMaxHealth()));
					if (GetHealth() < 0.03f) {
						PlayerCont->StartDie();
					}
				}
			}
		}
	}
}

void USunsetHollowBaseAttributeSet::Respawn()
{
	SetHealth(GetMaxHealth());
}
