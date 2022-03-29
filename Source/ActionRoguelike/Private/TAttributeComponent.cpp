// Fill out your copyright notice in the Description page of Project Settings.


#include "TAttributeComponent.h"

// Sets default values for this component's properties
UTAttributeComponent::UTAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	HealthMax = 100.f;
	Health = HealthMax;
}

bool UTAttributeComponent::ApplyHealthChange(float Delta)
{
	const float OldHealth = Health;
	Health += Delta;
	Health = FMath::Clamp(Health, 0.f, HealthMax);

	const float AppliedDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(nullptr, this, Health, AppliedDelta);
	
	return true;
}

bool UTAttributeComponent::IsAlive() const
{
	return Health > 0.f;
}

