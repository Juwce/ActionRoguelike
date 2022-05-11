// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPickupActor.h"
#include "TPickup_GrantActions.generated.h"

class UTAction;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ATPickup_GrantActions : public ATPickupActor
{
	GENERATED_BODY()

	virtual bool CanPickup(const APawn* InstigatorPawn) override;

	virtual void DoPickup(APawn* InstigatorPawn) override;

	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	TArray<TSubclassOf<UTAction>> GrantedActions;
};
