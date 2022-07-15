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

	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;

protected:
	virtual bool CanPickup(const APawn* InstigatorPawn);

	virtual void DoPickup(APawn* InstigatorPawn);
	
	UFUNCTION(BlueprintCallable)
	void Reactivate();

	void PlayInteractSound() const;

	void ApplyInteractionCredits(APawn* InstigatorPawn);
	
	UFUNCTION()
	void OnRep_bActive();
	
	UPROPERTY(ReplicatedUsing=OnRep_bActive)
	bool bActive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	USoundBase* InteractSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	float CooldownDuration;

	// Cost in credits required to interact with this pickup
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Credits")
	int32 InteractionCreditCost;

	// Reward in credits given when this pickup is successfully interacted with
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Credits")
	int32 InteractionCreditReward;

private:
	FTimerHandle CooldownTimerHandle;

};
