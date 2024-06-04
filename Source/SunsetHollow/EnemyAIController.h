// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"

class UAISenseConfig_Sight;

UCLASS()
class SUNSETHOLLOW_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController(FObjectInitializer const& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

private:
	UAISenseConfig_Sight* SightConfig;

	void SetupPerceptionSystem();
	void SetupSightConfig();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, const FAIStimulus Stimulus);
};
