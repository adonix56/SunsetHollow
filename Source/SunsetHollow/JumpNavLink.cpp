// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpNavLink.h"
#include "SunsetHollowCharacter.h"
#include "NavLinkCustomComponent.h"

void AJumpNavLink::BeginPlay()
{
	Super::BeginPlay();
	if (PointLinks.Num() < 1) {
		UE_LOG(LogTemp, Error, TEXT("JumpNavLink's Simple Links Array is empty. Please have at least 1."));
		return;
	}

	FVector SpawnLocation = GetActorLocation() + PointLinks[0].Left;
	FRotator SpawnRotation = FRotator();
	AInteractable* Interactable = GetWorld()->SpawnActor<AInteractable>(InteractableBP, SpawnLocation, SpawnRotation);
	Interactable->OnInteractEvent.BindDynamic(this, &AJumpNavLink::Jump);
}
void AJumpNavLink::Jump(AActor* InteractingActor)
{
	if (ASunsetHollowCharacter* PlayerCharacter = Cast<ASunsetHollowCharacter>(InteractingActor)) {
		FVector DestinationPoint = GetActorLocation() + PointLinks[0].Right;
		DestinationPoint.Z += 250.f;
		PlayerCharacter->JumpToDestination(DestinationPoint);
	}
}
