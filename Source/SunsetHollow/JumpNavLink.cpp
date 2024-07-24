// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpNavLink.h"
#include "SunsetHollowCharacter.h"
#include "NavLinkCustomComponent.h"

AJumpNavLink::AJumpNavLink(FObjectInitializer const& ObjectInitializer)
{
	//SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root Component")));
	OnSmartLinkReached.AddDynamic(this, &AJumpNavLink::SmartLinkReached);
	PointLinks.Empty();
	bSmartLinkIsRelevant = true;
}

void AJumpNavLink::SmartLinkReached(AActor* MovingActor, const FVector& DestinationPoint)
{
	if (ASunsetHollowCharacter* PlayerCharacter = Cast<ASunsetHollowCharacter>(MovingActor)) {
		PlayerCharacter->JumpToDestination(DestinationPoint);
	}
}
