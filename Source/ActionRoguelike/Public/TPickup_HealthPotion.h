// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPickupActor.h"
#include "TPickup_HealthPotion.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ATPickup_HealthPotion : public ATPickupActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATPickup_HealthPotion();

	UFUNCTION()
	bool HealPawn(const APawn* Pawn) const;

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HealthGiven;

};
