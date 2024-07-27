// Fill out your copyright notice in the Description page of Project Settings.


#include "SunsetHollowGameState.h"
#include "DialogueTableRow.h"

ASunsetHollowGameState::ASunsetHollowGameState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DialogueTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/UI/Dialogue"));
}

void ASunsetHollowGameState::ProgressGameState()
{
	int32 NextStateValue = static_cast<int32>(CurrentGameState) + 1;
	if (NextStateValue <= static_cast<int32>(EGameState::FREE_PLAY)) {
		SetGameState(static_cast<EGameState>(NextStateValue));
	}
}

FString ASunsetHollowGameState::GetDialogueAt(EGameState GameState, int Order)
{
	if (DialogueTable) {
		TArray<FName> RowNames = DialogueTable->GetRowNames();
		for (FName RowName : RowNames) {
			if (*RowName.ToString() == GetRowName(GameState, Order)) {
				FDialogueTableRow* Row = DialogueTable->FindRow<FDialogueTableRow>(RowName, TEXT("AGameStateGetRow"));
				if (Row) {
					return Row->Dialogue;
				}
			}
		}
	}
	return "";
}

FString ASunsetHollowGameState::GetNextDialogue()
{
	return FString();
}

void ASunsetHollowGameState::SetGameState(EGameState GameState)
{
	int32 StateValue = static_cast<int32>(GameState);
	UE_LOG(LogTemp, Warning, TEXT("Changing Gameplay State to %s"), *(StaticEnum<EGameState>()->GetDisplayNameTextByValue(StateValue).ToString()));
	CurrentGameState = GameState;
	OnGameStateChangedEvent.Broadcast(CurrentGameState);
}

FString ASunsetHollowGameState::GetRowName(EGameState GameState, int Order)
{
	int32 StateValue = static_cast<int32>(GameState);
	return FString::Printf(TEXT("%s%i"), *(StaticEnum<EGameState>()->GetDisplayNameTextByValue(StateValue).ToString()), Order);
}
