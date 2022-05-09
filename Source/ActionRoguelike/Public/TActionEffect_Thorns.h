// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TActionEffect.h"
#include "TActionEffect_Thorns.generated.h"

class UTAttributeComponent;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UTActionEffect_Thorns : public UTActionEffect
{
	GENERATED_BODY()

public:
	UTActionEffect_Thorns();

	virtual bool CanStart_Implementation(AActor* InstigatorActor) override;
	
	virtual void StartAction_Implementation(AActor* InstigatorActor) override;

	virtual void StopAction_Implementation(AActor* InstigatorActor) override;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UTAttributeComponent* OwnerComp, float NewHealth, float Delta);

	// The amount of damage to reflect (e.g. a ratio of 2.0 will reflect twice the amount of incoming damage)
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	float ReflectedDamageRatio;
};
