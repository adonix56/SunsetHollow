// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckDamageAnim.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CheckDamageAnim::UBTService_CheckDamageAnim()
{
	NodeName = TEXT("Check Damage Anim");
}

void UBTService_CheckDamageAnim::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (AEnemyAIController* EnemyCont = Cast<AEnemyAIController>(OwnerComp.GetAIOwner())) {
		if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(EnemyCont->GetPawn())) {
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), Enemy->IsInDamageAnim());
		}
	}
}
