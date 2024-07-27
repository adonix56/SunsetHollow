// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DialogueTableRow.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct SUNSETHOLLOW_API FDialogueTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dialogue)
	EGameState GameState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dialogue)
	int32 Order;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dialogue, meta = (MultiLine = "true"))
	FString Dialogue;
	
};
