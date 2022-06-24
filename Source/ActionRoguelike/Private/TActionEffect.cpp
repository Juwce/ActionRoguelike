// Fill out your copyright notice in the Description page of Project Settings.


#include "TActionEffect.h"

#include "TActionComponent.h"
#include "GameFramework/GameStateBase.h"

UTActionEffect::UTActionEffect()
{
	bAutoStart = true;
}

void UTActionEffect::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	if (Duration > 0.f)
	{
		FTimerDelegate DurationDelegate;
		DurationDelegate.BindUFunction(
			this, GET_FUNCTION_NAME_CHECKED(UTActionEffect, StopAction), InstigatorActor);
		GetWorld()->GetTimerManager().SetTimer(DurationHandle, DurationDelegate, Duration, false);
	}
	// ...else indefinite (until stopped)

	if (Period > 0.f)
	{
		FTimerDelegate PeriodDelegate;
		PeriodDelegate.BindUFunction(
			this, GET_FUNCTION_NAME_CHECKED(UTActionEffect, ExecutePeriodicEffect), InstigatorActor);
		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, PeriodDelegate, Period, true);
	}
}

void UTActionEffect::StopAction_Implementation(AActor* InstigatorActor)
{
	// If it was just about to trigger, trigger the periodic effect one last time before stopping
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(InstigatorActor);
	}
	
	Super::StopAction_Implementation(InstigatorActor);

	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);

	if (OwningActionComp)
	{
		OwningActionComp->RemoveAction(this);
	}
}

float UTActionEffect::GetTimeRemaining() const
{
	const AGameStateBase* GS = GetWorld()->GetGameState<AGameStateBase>();
	if (GS)
	{
		const float EndTime = TimeStarted + Duration;
		return EndTime - GS->GetServerWorldTimeSeconds();
	}

	return Duration;
}

void UTActionEffect::ExecutePeriodicEffect_Implementation(AActor* InstigatorActor)
{
}
