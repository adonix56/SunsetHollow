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
	GameState = Cast<ASunsetHollowGameState>(GetWorld()->GetGameState());
	if (ActivatingGameState != EGameState::NONE && GameState) {
		SetActorHiddenInGame(true);
		GameState->OnGameStateChangedEvent.AddDynamic(this, &AJumpNavLink::OnActivating);
	}
	else {
		CreateInteractable();
	}
	if (DeactivatingGameState != EGameState::NONE && GameState) {
		GameState->OnGameStateChangedEvent.AddDynamic(this, &AJumpNavLink::OnDeactivating);
	}
}
void AJumpNavLink::Jump(AActor* InteractingActor)
{
	if (ASunsetHollowCharacter* PlayerCharacter = Cast<ASunsetHollowCharacter>(InteractingActor)) {
		FVector DestinationPoint = GetActorLocation() + PointLinks[0].Right;
		DestinationPoint.Z += 250.f;
		PlayerCharacter->JumpToDestination(DestinationPoint);
	}
}

void AJumpNavLink::OnActivating(EGameState NewGameState)
{
	if (GameState && GameState->GetCurrentGameState() >= ActivatingGameState) {
		UE_LOG(LogTemp, Warning, TEXT("Activating JumpNavLink"));
		SetActorHiddenInGame(false);
		CreateInteractable();
		//GameState->OnGameStateChangedEvent.RemoveDynamic(this, &AJumpNavLink::OnGameStateChanged);
	}
}

void AJumpNavLink::OnDeactivating(EGameState NewGameState)
{
	if (GameState && GameState->GetCurrentGameState() >= DeactivatingGameState) {
		UE_LOG(LogTemp, Warning, TEXT("Deactivating JumpNavLink"));
		SetActorHiddenInGame(true);
		if (Interactable->Destroy()) Interactable = nullptr;
		//GameState->OnGameStateChangedEvent.RemoveDynamic(this, &AJumpNavLink::OnGameStateChanged);
	}
}

void AJumpNavLink::CreateInteractable()
{
	FVector SpawnLocation = PointLinks[0].Left;
	FRotator SpawnRotation = FRotator();
	Interactable = GetWorld()->SpawnActor<AInteractable>(InteractableBP, SpawnLocation, SpawnRotation);
	Interactable->OnInteractEvent.BindDynamic(this, &AJumpNavLink::Jump);
	Interactable->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
}
