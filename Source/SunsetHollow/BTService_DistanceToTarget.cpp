// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_DistanceToTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"

UBTService_DistanceToTarget::UBTService_DistanceToTarget(FObjectInitializer const& ObjectInitializer)
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Distance To Target");
}

void UBTService_DistanceToTarget::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	if (AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()))) {
		AActor* SelfActor = OwnerComp.GetAIOwner()->GetPawn();
		FVector SelfLocation = SelfActor->GetActorLocation();
		FVector TargetLocation = Target->GetActorLocation();
		float Distance = FVector::Dist2D(SelfLocation, TargetLocation);
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(TEXT("DistanceToTarget"), Distance);
	}
}
