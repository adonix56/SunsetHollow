// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickCastAbility.h"
#include "Kismet/KismetMathLibrary.h"

void UQuickCastAbility::TurnCharacterTowardsCursor(FVector& OutLocation, FVector& OutDirection) {
	FVector WorldLocation;
	FVector WorldDirection;
	if (GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(WorldLocation, WorldDirection)) {
		FHitResult OnHit;
		WorldDirection = (WorldDirection * 5000) + WorldLocation;
		if (GetWorld()->LineTraceSingleByChannel(OnHit, WorldLocation, WorldDirection, ECollisionChannel::ECC_Visibility)) {
			AActor* OwningActor = GetOwningActorFromActorInfo();
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(OwningActor->GetActorLocation(), OnHit.Location);
			OwningActor->SetActorRotation(LookAtRotation);

			OutLocation = OnHit.Location;
			OutDirection = OnHit.Location - OwningActor->GetActorLocation();
		}
	}
}