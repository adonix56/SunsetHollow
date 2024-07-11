// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_EnemyCastAbility.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../EnemyCharacter.h"
#include "../EnemyAIController.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

UBTTask_EnemyCastAbility::UBTTask_EnemyCastAbility(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Enemy Cast Ability");
	TEST = 1;
}

EBTNodeResult::Type UBTTask_EnemyCastAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn())) {
		if (Enemy->CastRandomAbility()) {
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Failed Casting Ability! %d"), TEST++);
	return EBTNodeResult::Failed;
}
