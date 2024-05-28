// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "QuickCastAbility.generated.h"

/**
 * 
 */
UCLASS()
class SUNSETHOLLOW_API UQuickCastAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable)
	void TurnCharacterTowardsCursor(FVector& OutLocation, FVector& OutDirection);
};
