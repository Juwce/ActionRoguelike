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

	bCheat_TakeNoDamage = false;
	bCheat_TakeAlmostNoDamage = false;
}

void UTAttributeComponent::ApplyHealthChange(float Delta)
{
	if (bCheat_TakeNoDamage)
	{
		Delta = FMath::Max(Delta, 0.f);
	}
	else if (bCheat_TakeAlmostNoDamage)
	{
		Delta = FMath::Max(Delta, -1.f);
	}
	
	const float OldHealth = Health;
	Health += Delta;
	Health = FMath::Clamp(Health, 0.f, HealthMax);

	const float AppliedDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(nullptr, this, Health, AppliedDelta);
}

void UTAttributeComponent::ApplyHealthChangeOverTime(const float Delta, const float Duration, const int32 Ticks)
{
	if (!ensure(Duration >= 0.f))
	{
		return;
	}
	if (!ensureMsgf(Ticks > 0, TEXT("Health Change over time must have at least 1 tick")))
	{
		return;
	}

	// TODO: Handle concurrent health change over time effects
	if (GetWorld()->GetTimerManager().IsTimerActive(HealthChangeTimerHandle))
	{
		return;
	}

	HealthChangeTicksLeft = Ticks;
	const float TickDelta = Delta / static_cast<float>(Ticks);
	const float TickDuration = Duration / static_cast<float>(Ticks);
	
	HealthChangeOverTime_Tick(TickDelta, TickDuration);
}

bool UTAttributeComponent::IsAlive() const
{
	return Health > 0.f;
}

void UTAttributeComponent::HealthChangeOverTime_Tick(const float Delta, const float Duration)
{
	if (HealthChangeTicksLeft <= 0)
	{
		return;
	}
	
	ApplyHealthChange(Delta);
	HealthChangeTicksLeft--;
	
	FTimerDelegate TimerDel;
	TimerDel.BindLambda([=]() { HealthChangeOverTime_Tick(Delta, Duration); });
	GetWorld()->GetTimerManager().SetTimer(HealthChangeTimerHandle, TimerDel, Duration, false);
}