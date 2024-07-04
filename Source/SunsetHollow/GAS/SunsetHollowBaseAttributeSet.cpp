// Fill out your copyright notice in the Description page of Project Settings.


#include "SunsetHollowBaseAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "../EnemyCharacter.h"
#include "../EnemyAIController.h"

void USunsetHollowBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute()) {
		const float DamageDealt = GetDamage();
		SetDamage(0.f);
		
		if (DamageDealt > 0.f) {
			if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Data.Target.GetOwner())) {
				if (AEnemyAIController* EnemyCont = Cast<AEnemyAIController>(Enemy->GetController())) {
					SetHealth(FMath::Clamp(GetHealth() - DamageDealt, 0.0f, GetMaxHealth()));
					if (GetHealth() < 0.03f) {
						EnemyCont->StartDie();
					}
				}
			}
		}
	}
}
