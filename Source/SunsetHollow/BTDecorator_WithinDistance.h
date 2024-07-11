// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_WithinDistance.generated.h"

/**
 * 
 */
UCLASS()
class SUNSETHOLLOW_API UBTDecorator_WithinDistance : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	float MinDistanceSquared;
};
