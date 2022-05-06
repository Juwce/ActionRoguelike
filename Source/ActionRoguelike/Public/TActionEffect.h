// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TAction.h"
#include "TActionEffect.generated.h"

/**
 * Action effects automatically start when added to a ActionComponent, execute at a fixed period over a specified
 * duration, finally removing themselves when the action is stopped or duration expires.
 */
UCLASS()
class ACTIONROGUELIKE_API UTActionEffect : public UTAction
{
	GENERATED_BODY()

	UTActionEffect();

	virtual void StartAction_Implementation(AActor* InstigatorActor) override;

	virtual void StopAction_Implementation(AActor* InstigatorActor) override;

protected:
	// The total duration the effect should last. Any value <= 0 will last indefinitely until StopAction() is called.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration;

	// The effect will be executed every "Period" seconds until "Duration" has elapsed or StopAction() is called. Must
	// be > 0.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect", meta=(ClampMin=0.0001))
	float Period;

	UFUNCTION(BlueprintNativeEvent)
	void ExecutePeriodicEffect(AActor* InstigatorActor);

	FTimerHandle DurationHandle;
	
	FTimerHandle PeriodHandle;
};
