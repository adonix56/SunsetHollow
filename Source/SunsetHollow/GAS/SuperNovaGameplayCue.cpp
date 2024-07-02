// Fill out your copyright notice in the Description page of Project Settings.


#include "SuperNovaGameplayCue.h"
#include "Particles/ParticleSystemComponent.h"

ASuperNovaGameplayCue::ASuperNovaGameplayCue()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true; 
	bAutoDestroyOnRemove = true;
	AutoDestroyDelay = 2.0f;
	GameplayCueTag = FGameplayTag::RequestGameplayTag(TEXT("GameplayCue.SuperNova"));
	bAutoAttachToOwner = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneRoot);

	Blast1 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Blast 1"));
	Blast2 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Blast 2"));
	Blast3 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Blast 3"));
	Blast4 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Blast 4"));
	BlastParticleSystems.Add(Blast1);
	BlastParticleSystems.Add(Blast2);
	BlastParticleSystems.Add(Blast3);
	BlastParticleSystems.Add(Blast4);
	for (UParticleSystemComponent* Blast : BlastParticleSystems) {
		Blast->SetupAttachment(SceneRoot);
		Blast->PrimaryComponentTick.bStartWithTickEnabled = false;
		Blast->bAutoActivate = false;
	}
	Blast1->SetRelativeLocation(FVector(200.f, 0.f, 0.f));
	Blast2->SetRelativeLocation(FVector(0.f, 200.f, 0.f));
	Blast3->SetRelativeLocation(FVector(-200.f, 0.f, 0.f));
	Blast4->SetRelativeLocation(FVector(0.f, -200.f, 0.f));

	RepeatBlastParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RepeatBlasts"));
	RepeatBlastParticleSystem->SetupAttachment(SceneRoot);
	RepeatBlastParticleSystem->PrimaryComponentTick.bStartWithTickEnabled = false;
	RepeatBlastParticleSystem->bAutoActivate = true;
}

void ASuperNovaGameplayCue::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (PlayAnim) {
		AnimTimeElapsed += DeltaSeconds;
		RepeatBlastElapsed += DeltaSeconds;
		if (AnimTimeElapsed > FinalBlastDelay) {
			for (UParticleSystemComponent* Blast : BlastParticleSystems) {
				Blast->SetActive(true, true);
			}
			PlayAnim = false;
		}
		else if (RepeatBlastElapsed > RepeatBlastInterval) {
			RepeatBlastElapsed = 0.f;
			RepeatBlastParticleSystem->SetActive(true, true);
		}
	}
}

bool ASuperNovaGameplayCue::OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	PlayAnim = true;
	RepeatBlastParticleSystem->SetActive(true, true);
	return false;
}

bool ASuperNovaGameplayCue::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	RepeatBlastParticleSystem->SetActive(false);
	for (UParticleSystemComponent* Blast : BlastParticleSystems) {
		Blast->SetActive(false);
	}
	AnimTimeElapsed = 0.0f;
	RepeatBlastElapsed = 0.0f;
	return false;
}
