// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SunsetHollowCharacter.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GASComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("GAS Component"));
	DamagedTagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Damaged")));
}

void AEnemyCharacter::HandleDamageAnimation(DamageAppliedType DamageType, float DistanceMoved, FVector LaunchDirection)
{
	switch (DamageType) {
	case DamageAppliedType::KNOCKUP:
		PlayAnim = KnockupAnim;
		break;
	case DamageAppliedType::KNOCKBACK:
		PlayAnim = KnockbackAnim;
		break;
	}
	if (PlayAnim == nullptr || DamageType == DamageAppliedType::BASIC) {
		PlayAnim = BasicAnim;
	}

	if (PlayAnim) {
		StopAnimMontage();
		PlayAnimMontage(PlayAnim);
		if (DistanceMoved > 0.f && LaunchDirection.Length() > 0.f) {
			if (ASunsetHollowCharacter* PlayerCharacter = Cast<ASunsetHollowCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())) {
				//FVector LaunchDirection = (GetActorLocation() - PlayerCharacter->GetActorLocation()).GetSafeNormal2D(0.002);
				SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerCharacter->GetActorLocation()));
				LaunchDirection.Z = 0.05f;
				UE_LOG(LogTemp, Warning, TEXT("Launch %f, %f, %f"), LaunchDirection.X, LaunchDirection.Y, LaunchDirection.Z);
				LaunchCharacter(LaunchDirection * DistanceMoved, true, false);
			}
		}
	}
}

bool AEnemyCharacter::IsInDamageAnim()
{
	if (IsValid(GASComponent) && !DamagedTagContainer.IsEmpty()) {
		return GASComponent->GetActiveEffectsWithAllTags(DamagedTagContainer).Num() > 0;
	}
	return false;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(GASComponent)) {
		BaseAttributeSet = GASComponent->GetSet<USunsetHollowBaseAttributeSet>();
		if (BaseAttributeSet) {
			GASComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetHealthAttribute()).AddUObject(this, &AEnemyCharacter::HealthChanged);
		}
	}
}

void AEnemyCharacter::HealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Health Changed by %f"), Data.NewValue);
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

