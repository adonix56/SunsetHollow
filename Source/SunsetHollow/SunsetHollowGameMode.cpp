// Copyright Epic Games, Inc. All Rights Reserved.

#include "SunsetHollowGameMode.h"
#include "SunsetHollowPlayerController.h"
#include "SunsetHollowCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASunsetHollowGameMode::ASunsetHollowGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ASunsetHollowPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}

void ASunsetHollowGameMode::SetRespawnTransform(FTransform NewTransform)
{
	RespawnTransform = NewTransform;
}

void ASunsetHollowGameMode::RespawnPlayer(ACharacter* Player)
{
	const FTransform* SpawnTransform = &RespawnTransform;
	Player->SetActorTransform(RespawnTransform);
}
