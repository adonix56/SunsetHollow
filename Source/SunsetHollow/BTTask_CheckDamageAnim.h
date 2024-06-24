// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_CheckDamageAnim.generated.h"

/**
 * 
 */
UCLASS()
class SUNSETHOLLOW_API UBTTask_CheckDamageAnim : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UBTTask_CheckDamageAnim(FObjectInitializer const& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
