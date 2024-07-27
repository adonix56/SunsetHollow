// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SunsetHollowGameState.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8 {
	NONE = 0 UMETA(DisplayName = "None"),
	TUTORIAL_MOVEMENT = 1 UMETA(DisplayName = "Movement"),
	TUTORIAL_AA = 2 UMETA(DisplayName = "Auto Attack"),
	TUTORIAL_DASH = 3 UMETA(DisplayName = "Dashing"),
	TUTORIAL_SKILLS = 4 UMETA(DisplayName = "Skills"),
	FREE_PLAY = 5 UMETA(DisplayName = "Free Play")
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
	const float TutorialMovementTotal = 1000.f;
	UPROPERTY(BlueprintAssignable)
	FOnGameStateChanged OnGameStateChangedEvent;

	EGameState GetCurrentGameState() const { return CurrentGameState; }
	UFUNCTION()
	void ProgressGameState();

protected:
	EGameState CurrentGameState = EGameState::TUTORIAL_MOVEMENT;
};
