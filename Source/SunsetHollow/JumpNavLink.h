// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/NavLinkProxy.h"
#include "Components/BoxComponent.h"
#include "Interactable.h"
#include "ActivatingInterface.h"
#include "JumpNavLink.generated.h"

/**
 * 
 */
UCLASS()
class SUNSETHOLLOW_API AJumpNavLink : public ANavLinkProxy, public IActivatingInterface
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameState, meta = (AllowPrivateAccess = "true"))
	EGameState ActivatingGameState;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameState, meta = (AllowPrivateAccess = "true"))
	EGameState DeactivatingGameState;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameState, meta = (AllowPrivateAccess = "true"))
	bool bProgressGameStateAfterInteracting;
	ASunsetHollowGameState* GameState;
	AInteractable* Interactable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interactable, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AInteractable> InteractableBP;

	UFUNCTION()
	void Jump(AActor* InteractingActor);
	UFUNCTION()
	void OnActivating(EGameState NewGameState) override;
	UFUNCTION()
	void OnDeactivating(EGameState NewGameState) override;
	void CreateInteractable();
};
