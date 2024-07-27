// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SunsetHollowGameState.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8 {
	NONE = 0 UMETA(DisplayName = "None"),
	TUTORIAL_MOVEMENT = 1 UMETA(DisplayName = "Movement"),
	TUTORIAL_AA = 2 UMETA(DisplayName = "AutoAttack"),
	TUTORIAL_DASH = 3 UMETA(DisplayName = "Dashing"),
	TUTORIAL_SKILLS = 4 UMETA(DisplayName = "Skills"),
	FREE_PLAY = 5 UMETA(DisplayName = "FreePlay")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameStateChanged, EGameState, ChangedGameState);

/**
 * 
 */
UCLASS()
class SUNSETHOLLOW_API ASunsetHollowGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ASunsetHollowGameState(const FObjectInitializer& ObjectInitializer);

	const float TutorialMovementTotal = 1000.f;
	UPROPERTY(BlueprintAssignable)
	FOnGameStateChanged OnGameStateChangedEvent;

	UFUNCTION(BlueprintCallable)
	EGameState GetCurrentGameState() const { return CurrentGameState; }
	UFUNCTION(BlueprintCallable)
	void ProgressGameState();
	UFUNCTION(BlueprintCallable)
	FString GetDialogueAt(EGameState GameState, int Order);
	UFUNCTION(BlueprintCallable)
	FString GetNextDialogue();

protected:
	EGameState CurrentGameState = EGameState::TUTORIAL_MOVEMENT;
	void SetGameState(EGameState GameState);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Dialogue, meta = (AllowPrivateAccess = "true"))
	UDataTable* DialogueTable;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Dialogue, meta = (AllowPrivateAccess = "true"))
	int CurrentDialogueOrder;
	FString GetRowName(EGameState GameState, int Order);
};
