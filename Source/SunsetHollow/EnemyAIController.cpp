// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyCharacter.h"
#include "SunsetHollowCharacter.h"

AEnemyAIController::AEnemyAIController(FObjectInitializer const& ObjectInitializer)
{
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(InPawn)) {
		if (UBehaviorTree* EnemyTree = Enemy->GetBehaviorTree()) {
			UBlackboardComponent* BlackboardTemp;
			UseBlackboard(EnemyTree->BlackboardAsset, BlackboardTemp);
			Blackboard = BlackboardTemp;
			RunBehaviorTree(EnemyTree);
		}
	}
}
