// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/NavLinkProxy.h"
#include "Components/BoxComponent.h"
#include "Interactable.h"
#include "JumpNavLink.generated.h"

/**
 * 
 */
UCLASS()
class SUNSETHOLLOW_API AJumpNavLink : public ANavLinkProxy
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interactable, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AInteractable> InteractableBP;

	UFUNCTION()
	void Jump(AActor* InteractingActor);

};
