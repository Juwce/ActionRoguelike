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

protected:
	virtual bool CanPickup(const APawn* InstigatorPawn) override;

	virtual void DoPickup(APawn* InstigatorPawn) override;
	
	void HealPawn(const APawn* Pawn);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HealthGiven;

};
