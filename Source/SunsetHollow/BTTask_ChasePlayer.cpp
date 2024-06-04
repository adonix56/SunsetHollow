// Fill out your copyright notice in the Description page of Project Settings.

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EnemyAIController.h"
#include "BTTask_ChasePlayer.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ChasePlayer::UBTTask_ChasePlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	if (AEnemyAIController* EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner())) {
		FVector PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(EnemyController, PlayerLocation);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
