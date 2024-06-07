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

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	SetupStimulusSource();
}

void ASunsetHollowCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ASunsetHollowCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GASComponent)) {
		BaseAttributeSet = GASComponent->GetSet<USunsetHollowBaseAttributeSet>();
	}
	//AbilitySystemComponent->InitAbilityActorInfo(this, this);

	//GASComponent->TryActivateAbilityByClass(TSubclassOf<>)
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
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, IgnoreCollision ? ECollisionResponse::ECR_Overlap : ECollisionResponse::ECR_Block);
}
