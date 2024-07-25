// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/Color.h"
#include "Interactable.generated.h"

class UWidgetComponent;
class UBoxComponent;
class UParticleSystemComponent;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnInteract, AActor*, InteractingActor);

UCLASS()
class SUNSETHOLLOW_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	UFUNCTION(BlueprintCallable)
	void Interact();

	UPROPERTY()
	FOnInteract OnInteractEvent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* DisplayWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* InteractTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* ParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	FLinearColor ParticleSystemColor;

	AActor* InteractingActor;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
