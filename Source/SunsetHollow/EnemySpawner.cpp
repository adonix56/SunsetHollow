// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) {
		GameState = Cast<ASunsetHollowGameState>(GetWorld()->GetGameState());
		if (ActivatingState != EGameState::NONE && GameState) {
			GameState->OnGameStateChangedEvent.AddDynamic(this, &AEnemySpawner::OnActivating);
		}
		else {
			OnActivating(ActivatingState);
		}
		if (DeactivatingState != EGameState::NONE && GameState) {
			GameState->OnGameStateChangedEvent.AddDynamic(this, &AEnemySpawner::OnDeactivating);
		}
	}
}

void AEnemySpawner::Spawn()
{
	if (EnemyToSpawn == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("EnemySpawner: Enemy to Spawn has not been set."));
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AEnemyCharacter* SpawnedEnemy = GetWorld()->SpawnActor<AEnemyCharacter>(EnemyToSpawn, GetActorTransform(), SpawnParams);
	SpawnedEnemy->OnEnemyDefeatedEvent.AddDynamic(this, &AEnemySpawner::OnEnemyDefeated);
	SpawnedEnemies.Add(SpawnedEnemy, SpawnedEnemy);
}

void AEnemySpawner::OnEnemyDefeated(AEnemyCharacter* EnemyDefeated)
{
	SpawnedEnemies.Remove(EnemyDefeated);
	if (bProgressGameStateAfterEnemiesKilled && SpawnedEnemies.Num() == 0) {
		GetWorldTimerManager().ClearTimer(TimerHandle);
		GameState->ProgressGameState();
	}
}

void AEnemySpawner::OnActivating(EGameState NewGameState)
{
	if (NewGameState == ActivatingState) {
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemySpawner::Spawn, RespawnTimer, bRepeatable, 0.1f);
		GameState->OnGameStateChangedEvent.RemoveDynamic(this, &AEnemySpawner::OnActivating);
	}
}

void AEnemySpawner::OnDeactivating(EGameState NewGameState)
{
	if (NewGameState == DeactivatingState) {
		GameState->OnGameStateChangedEvent.RemoveDynamic(this, &AEnemySpawner::OnDeactivating);
		Destroy();
	}
}

