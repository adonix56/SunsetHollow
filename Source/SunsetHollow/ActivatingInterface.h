// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SunsetHollowGameState.h"
#include "ActivatingInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UActivatingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SUNSETHOLLOW_API IActivatingInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION()
	virtual void OnActivating(EGameState NewGameState) = 0;
	UFUNCTION()
	virtual void OnDeactivating(EGameState NewGameState) = 0;
};
