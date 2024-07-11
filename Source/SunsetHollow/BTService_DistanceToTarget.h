// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_DistanceToTarget.generated.h"

/**
 * 
 */
UCLASS()
class SUNSETHOLLOW_API UBTService_DistanceToTarget : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_DistanceToTarget();

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
