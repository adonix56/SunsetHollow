// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/NavLinkProxy.h"
#include "Components/BoxComponent.h"
#include "JumpNavLink.generated.h"

/**
 * 
 */
UCLASS()
class SUNSETHOLLOW_API AJumpNavLink : public ANavLinkProxy
{
	GENERATED_BODY()
	
public:

	AJumpNavLink(FObjectInitializer const& ObjectInitializer);

protected:
	UFUNCTION()
	void SmartLinkReached(AActor* MovingActor, const FVector& DestinationPoint);

};
