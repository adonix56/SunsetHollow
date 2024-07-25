// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "SunsetHollowCharacter.h"
#include "SunsetHollowPlayerController.h"

// Sets default values
AInteractable::AInteractable()
{
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root Component")));
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DisplayWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Display Widget"));
	DisplayWidget->SetupAttachment(RootComponent);
	DisplayWidget->SetWidgetSpace(EWidgetSpace::Screen);
	DisplayWidget->SetVisibility(false, true);

	InteractTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Interact Trigger"));
	InteractTrigger->SetupAttachment(RootComponent);

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System"));
	ParticleSystem->SetupAttachment(RootComponent);
}

void AInteractable::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (ASunsetHollowCharacter* PlayerCharacter = Cast<ASunsetHollowCharacter>(OtherActor)) {
		if (ASunsetHollowPlayerController* PlayerCont = Cast<ASunsetHollowPlayerController>(PlayerCharacter->GetController())) {
			DisplayWidget->SetVisibility(true, true);
			PlayerCont->SetInteractable(this);
			InteractingActor = OtherActor;
		}
	}
}

void AInteractable::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (ASunsetHollowCharacter* PlayerCharacter = Cast<ASunsetHollowCharacter>(OtherActor)) {
		if (ASunsetHollowPlayerController* PlayerCont = Cast<ASunsetHollowPlayerController>(PlayerCharacter->GetController())) {
			DisplayWidget->SetVisibility(false, true);
			PlayerCont->RemoveInteractable(this);
			InteractingActor = nullptr;
		}
	}
}

void AInteractable::Interact()
{
	OnInteractEvent.ExecuteIfBound(InteractingActor);
	UE_LOG(LogTemp, Warning, TEXT("Interactable is called!"));
}

// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();
	ParticleSystem->SetVectorParameter(FName("color"), FVector(ParticleSystemColor));
}

// Called every frame
void AInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

