// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyCharacter.h"
#include "ActivatingInterface.h"
#include "EnemySpawner.generated.h"

UCLASS()
class SUNSETHOLLOW_API AEnemySpawner : public AActor, public IActivatingInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Spawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Respawn, meta = (AllowPrivateAccess = "true"))
	float RespawnTimer = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Respawn, meta = (AllowPrivateAccess = "true"))
	bool bRepeatable;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Respawn, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AEnemyCharacter> EnemyToSpawn;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Respawn, meta = (AllowPrivateAccess = "true"))
	TMap<AEnemyCharacter*, AEnemyCharacter*> SpawnedEnemies;
	FTimerHandle TimerHandle;

	ASunsetHollowGameState* GameState;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameState, meta = (AllowPrivateAccess = "true"))
	EGameState ActivatingState;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameState, meta = (AllowPrivateAccess = "true"))
	EGameState DeactivatingState;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameState, meta = (AllowPrivateAccess = "true"))
	bool bProgressAfterEnemiesKilled;
	UFUNCTION()
	void OnEnemyDefeated(AEnemyCharacter* EnemyDefeated);
	UFUNCTION()
	virtual void OnActivating(EGameState NewGameState) override;
	UFUNCTION()
	virtual void OnDeactivating(EGameState NewGameState) override;
};
