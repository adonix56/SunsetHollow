// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_WithinDistance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SunsetHollowCharacter.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"

bool UBTDecorator_WithinDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UObject* KeyObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey());
	if (ASunsetHollowCharacter* PlayerCharacter = Cast<ASunsetHollowCharacter>(KeyObject)) {
		if (AEnemyCharacter* OwnerEnemy = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn())) {
			float PlayerEnemyDistSq = FVector::DistSquared2D(OwnerEnemy->GetActorLocation(), PlayerCharacter->GetActorLocation());
			return PlayerEnemyDistSq > MinDistanceSquared;
		}
	}
	return false;
}
