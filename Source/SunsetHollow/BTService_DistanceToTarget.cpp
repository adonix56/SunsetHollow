// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_DistanceToTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTService_DistanceToTarget::UBTService_DistanceToTarget()
{
	NodeName = TEXT("Distance To Target");
}

void UBTService_DistanceToTarget::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	if (AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()))) {
		FVector SelfLocation = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
		FVector TargetLocation = Target->GetActorLocation();
		float Distance = FVector::Dist2D(SelfLocation, TargetLocation);
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(TEXT("DistanceToTarget"), Distance);
	}
}
