// Fill out your copyright notice in the Description page of Project Settings.


#include "Storm_Projectile.h"
#include "Components/CapsuleComponent.h"
#include "../SunsetHollowCharacter.h"
#include "AbilitySystemComponent.h"
#include "../EnemyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

// Sets default values
AStorm_Projectile::AStorm_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneRootComponent;
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Hitbox"));
	CapsuleComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CapsuleComponent->SetRelativeLocation(FVector(0.f, 0.f, 190.f));
	CapsuleComponent->SetCapsuleHalfHeight(190.f);
	CapsuleComponent->SetCapsuleRadius(80.f);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	Storm = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Storm"));
	Storm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Storm->SetWorldScale3D(FVector(0.0f));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = 400.f;
	ProjectileMovement->MaxSpeed = 500.f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
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
		//DamageSpec.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Event.Damage")), Damage * -1);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpec, FGameplayTag::RequestGameplayTag(FName("Event.Damage")), Damage);
	}
}

// Called every frame
void AStorm_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//Deal damage at the end of every interval
	if (DamageInterval > 0 && (TimeSinceLastDamage == 0 || TimeSinceLastDamage > DamageInterval)) {
		TArray<AActor*> HitActors;
		CapsuleComponent->GetOverlappingActors(HitActors, AEnemyCharacter::StaticClass());
		for (AActor* HitActor : HitActors) {
			AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(HitActor);
			if (Enemy) {
				PlayerAbilitySystem->ApplyGameplayEffectSpecToTarget(*DamageSpec.Data.Get(), Enemy->GetAbilitySystemComponent());
				Enemy->HandleDamageAnimation(EDamageAppliedType::KNOCKUP, 0.0f, FVector::ZeroVector, false);
			}
		}
		TimeSinceLastDamage = 0;
	}
	
	// At the beginning, we grow the scale
	FVector CurrentScale = Storm->GetComponentScale();
	if (UKismetMathLibrary::SafeDivide(MaxScale, GrowthSpeed) < (Lifespan - Timer)) {
		if (CurrentScale.X < MaxScale) {
			Storm->SetWorldScale3D(CurrentScale + (GrowthSpeed * DeltaTime));
		}
		else {
			Storm->SetWorldScale3D(FVector(MaxScale));
		}
	}
	else { // At the end, we shrink the scale
		if (CurrentScale.X > 0) {
			Storm->SetWorldScale3D(CurrentScale - FVector(GrowthSpeed * DeltaTime));
		}
		else {
			Storm->SetWorldScale3D(FVector(0.f));
		}
	}

	TimeSinceLastDamage += DeltaTime;
	Timer += DeltaTime;
}

