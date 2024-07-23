// Copyright Epic Games, Inc. All Rights Reserved.

#include "SunsetHollowCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "AbilitySystemComponent.h"
#include "Engine/World.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "SunsetHollowGameMode.h"

ASunsetHollowCharacter::ASunsetHollowCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	GASComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("GASAbilitySystemComponent"));

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	TopDownCameraComponent->PostProcessSettings.ColorGamma = FVector4(0.7f, 0.7f, 0.7f);
	TopDownCameraComponent->PostProcessSettings.ColorSaturation = FVector4();

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	SetupStimulusSource();
}

void ASunsetHollowCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
	if (bZoom) {
		if (ElapsedTime < ZoomTime) {
			float t = ElapsedTime / ZoomTime;
			float start = 1400.f;
			if (bReverse) CameraBoom->TargetArmLength = FMath::Lerp(TargetDistance, start, t);
			else CameraBoom->TargetArmLength = FMath::Lerp(start, TargetDistance, t);
			ElapsedTime += DeltaSeconds;
		}
		else if (Delay > 0) {
			Delay -= DeltaSeconds;
		}
		else if (!bReverse) {
			ElapsedTime = 0.f;
			bReverse = true;
		}
		else {
			bZoom = false;
			bReverse = false;
		}
	}
}

void ASunsetHollowCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GASComponent)) {
		BaseAttributeSet = GASComponent->GetSet<USunsetHollowBaseAttributeSet>();
		GASComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetHealthAttribute()).AddUObject(this, &ASunsetHollowCharacter::HealthChanged);
	}
	//AbilitySystemComponent->InitAbilityActorInfo(this, this);

	//GASComponent->TryActivateAbilityByClass(TSubclassOf<>)
}

void ASunsetHollowCharacter::HealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Health has changed by %f"), Data.NewValue);
	HealthChangeEvent.Broadcast(Data.NewValue);
}

void ASunsetHollowCharacter::SetupStimulusSource()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus Source Component"));
	if (StimulusSource) {
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->RegisterWithPerceptionSystem();
	}
}

void ASunsetHollowCharacter::IncreaseAttackCount() {
	if (AttackCount == 2) AttackCount = 0;
	else AttackCount++;
}

void ASunsetHollowCharacter::IgnoreEnemyCollision(bool IgnoreCollision)
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, IgnoreCollision ? ECollisionResponse::ECR_Overlap : ECollisionResponse::ECR_Block);
}

void ASunsetHollowCharacter::ZoomCameraWithTimer(float zoomTime, float targetDistance, float speed, float delay)
{
	ElapsedTime = 0.f;
	ZoomTime = zoomTime;
	TargetDistance = targetDistance;
	Delay = delay;
	bZoom = true;
}

void ASunsetHollowCharacter::StartDie()
{
	bIsDead = true;
	GASComponent->CancelAllAbilities();
	GetMesh()->GetAnimInstance()->Montage_Stop(0.2f);
	TopDownCameraComponent->PostProcessSettings.bOverride_ColorSaturation = true;
	TopDownCameraComponent->PostProcessSettings.bOverride_ColorGamma = true;
	CharacterDeathEvent.Broadcast(this);
	//PlayAnimMontage(DieAnim);
}

void ASunsetHollowCharacter::StartRespawn()
{
	bIsRespawning = true;
	TopDownCameraComponent->PostProcessSettings.bOverride_ColorSaturation = false;
	TopDownCameraComponent->PostProcessSettings.bOverride_ColorGamma = false;

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASunsetHollowCharacter::EndRespawn, 2.03f, false);

	const_cast<USunsetHollowBaseAttributeSet*>(BaseAttributeSet)->Respawn();
}

void ASunsetHollowCharacter::EndRespawn()
{
	bIsDead = false;
	bIsRespawning = false;
	//GetMovementComponent()->Activate(true);
}
