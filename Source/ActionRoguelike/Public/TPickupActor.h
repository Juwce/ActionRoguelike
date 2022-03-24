// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TGameplayInterface.h"
#include "TPickupActor.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ATPickupActor : public AActor, public ITGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATPickupActor();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:

	UFUNCTION(BlueprintCallable)
	void Activate();

	UFUNCTION(BlueprintCallable)
	void DeactivateAndCooldown();

	void PlayInteractSound();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	USoundBase* InteractSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	float CooldownDuration;

	FTimerHandle CooldownTimerHandle;

};
