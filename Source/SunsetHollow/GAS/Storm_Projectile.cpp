// Fill out your copyright notice in the Description page of Project Settings.


#include "Storm_Projectile.h"
#include "Components/CapsuleComponent.h"
#include "../SunsetHollowCharacter.h"
#include "AbilitySystemComponent.h"

// Sets default values
AStorm_Projectile::AStorm_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Hitbox"));
	CapsuleComponent->SetRelativeLocation(FVector(0.f, 0.f, 190.f));
	CapsuleComponent->SetCapsuleHalfHeight(190.f);
	CapsuleComponent->SetCapsuleRadius(80.f);
	InitialLifeSpan = Lifespan;
}

// Called when the game starts or when spawned
void AStorm_Projectile::BeginPlay()
{
	Super::BeginPlay();
	ASunsetHollowCharacter* PlayerCharacter = Cast<ASunsetHollowCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (PlayerCharacter) {
		PlayerAbilitySystem = PlayerCharacter->GetAbilitySystemComponent();
		DamageSpec = PlayerAbilitySystem->MakeOutgoingSpec(GE_DealDamage, 0, PlayerAbilitySystem->MakeEffectContext());
		//Spec.SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Event.Damage")), Damage * -1);
		DamageSpec.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Event.Damage")), Damage * -1);
	}
}

// Called every frame
void AStorm_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimeSinceLastDamage += DeltaTime;
	Timer += DeltaTime;
	
	//Deal damage at the end of every interval
	if (TimeSinceLastDamage == 0 || TimeSinceLastDamage > DamageInterval) {

	}
}

