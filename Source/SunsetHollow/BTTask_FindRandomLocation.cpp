// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindRandomLocation.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Random Location in NavMesh");
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AEnemyAIController* EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner())) {
		if (APawn* EnemyPawn = EnemyController->GetPawn()) {
			FVector CurrentLocation(EnemyPawn->GetActorLocation());
			if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld())) {
				FNavLocation TargetLocation;
				if (NavSys->GetRandomPointInNavigableRadius(CurrentLocation, SearchRadius, TargetLocation)) {
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), TargetLocation);
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}
