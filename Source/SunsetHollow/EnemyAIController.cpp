// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "EnemyCharacter.h"
#include "SunsetHollowCharacter.h"

AEnemyAIController::AEnemyAIController(FObjectInitializer const& ObjectInitializer)
{
	SetupPerceptionSystem();
}

void AEnemyAIController::StartDie()
{
	if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(GetPawn())) {
		Enemy->StartDie();
		UnPossess();
		Destroy();
	}
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(InPawn)) {
		if (UBehaviorTree* EnemyTree = Enemy->GetBehaviorTree()) {
			UBlackboardComponent* BlackboardTemp;
			UseBlackboard(EnemyTree->BlackboardAsset, BlackboardTemp);
			Blackboard = BlackboardTemp;
			//Blackboard->SetValueAsObject(FName("GASComponent"), Enemy->GetAbilitySystemComponent());
			RunBehaviorTree(EnemyTree);
			SetupSightConfig();
		}
	}
}

void AEnemyAIController::OnPlayerDeath(ASunsetHollowCharacter* PlayerCharacter)
{
	GetBlackboardComponent()->SetValueAsObject("Player", nullptr);
	PlayerCharacter->CharacterDeathEvent.Remove(DeathDelegateHandle);
}

void AEnemyAIController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig) {
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		SightConfig->SightRadius = 0.f;
		SightConfig->LoseSightRadius = 0.f;
		SightConfig->PeripheralVisionAngleDegrees = 360.f;
		SightConfig->SetMaxAge(5.f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 500.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetDetected);
	}
}

void AEnemyAIController::SetupSightConfig()
{
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(GetPawn());
	if (SightConfig && Enemy) {
		SightConfig->SightRadius = Enemy->GetSightRadius();
		SightConfig->LoseSightRadius = Enemy->GetLoseSightRadius();
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}
}

void AEnemyAIController::OnTargetDetected(AActor* Actor, const FAIStimulus Stimulus)
{
	if (ASunsetHollowCharacter* Player = Cast<ASunsetHollowCharacter>(Actor)) {
		bool Sensed = !Player->IsDying() && Stimulus.WasSuccessfullySensed();
		if (Sensed) {
			DeathDelegateHandle = Player->CharacterDeathEvent.AddUObject(this, &AEnemyAIController::OnPlayerDeath);
		}
		else {
			Player->CharacterDeathEvent.Remove(DeathDelegateHandle);
		}
		GetBlackboardComponent()->SetValueAsObject("Player", Sensed ? Cast<ASunsetHollowCharacter>(Actor) : nullptr);
	}
}
