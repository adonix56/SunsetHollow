// Fill out your copyright notice in the Description page of Project Settings.


#include "SunsetHollowGameState.h"

void ASunsetHollowGameState::ProgressGameState()
{
	int32 NextStateValue = static_cast<int32>(CurrentGameState) + 1;
	if (NextStateValue <= static_cast<int32>(EGameState::FREE_PLAY)) {
		UE_LOG(LogTemp, Warning, TEXT("Changing Gameplay State to %s"), *StaticEnum<EGameState>()->GetNameStringByValue(NextStateValue));
		CurrentGameState = static_cast<EGameState>(NextStateValue);
		OnGameStateChangedEvent.Broadcast(CurrentGameState);
	}
}
