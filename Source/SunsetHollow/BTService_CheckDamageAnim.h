// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CheckDamageAnim.generated.h"

/**
 * 
 */
UCLASS()
class SUNSETHOLLOW_API UBTService_CheckDamageAnim : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_CheckDamageAnim();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


	
};
