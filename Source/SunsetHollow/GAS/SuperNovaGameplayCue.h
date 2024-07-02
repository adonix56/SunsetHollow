// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "SuperNovaGameplayCue.generated.h"

class UParticleSystemComponent;
/**
 * 
 */
UCLASS()
class SUNSETHOLLOW_API ASuperNovaGameplayCue : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()

public:
	ASuperNovaGameplayCue();

	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneComponent* SceneRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ParticleSystem)
	TArray<UParticleSystemComponent*> BlastParticleSystems;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ParticleSystem)
	UParticleSystemComponent* Blast1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ParticleSystem)
	UParticleSystemComponent* Blast2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ParticleSystem)
	UParticleSystemComponent* Blast3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ParticleSystem)
	UParticleSystemComponent* Blast4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ParticleSystem)
	UParticleSystemComponent* RepeatBlastParticleSystem;

	bool OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
	bool OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;

	bool PlayAnim;
	float AnimTimeElapsed;
	float RepeatBlastElapsed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ParticleSystem)
	float RepeatBlastInterval;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ParticleSystem)
	float FinalBlastDelay;
};
