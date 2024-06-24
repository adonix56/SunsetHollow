// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CheckDamageAnim.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyCharacter.h"

UBTTask_CheckDamageAnim::UBTTask_CheckDamageAnim(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Check Damage Anim");
}

EBTNodeResult::Type UBTTask_CheckDamageAnim::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AEnemyAIController* EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner())) {
		if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(EnemyController->GetPawn())) {
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), Enemy->IsInDamageAnim);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
