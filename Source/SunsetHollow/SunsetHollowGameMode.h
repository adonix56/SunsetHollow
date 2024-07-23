// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SunsetHollowGameMode.generated.h"

UCLASS(minimalapi)
class ASunsetHollowGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASunsetHollowGameMode();

	UFUNCTION(BlueprintCallable)
	void SetRespawnTransform(FTransform NewTransform);

	void RespawnPlayer(ACharacter* Player);

protected:

	FTransform RespawnTransform;
};
