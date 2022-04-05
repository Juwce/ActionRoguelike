// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, UTAttributeComponent*, OwningComp, float, NewHealth, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UTAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTAttributeComponent();

	float GetHealth() { return Health; }
	float GetHealthMax() { return HealthMax; }

	UFUNCTION(BlueprintCallable)
	void ApplyHealthChange(float Delta);

	// Apply a health change over the given duration, split up evenly over the specified number of ticks
	UFUNCTION(BlueprintCallable)
	void ApplyHealthChangeOverTime(float Delta, float Duration, int32 Ticks);

	// Immediately stop any ongoing health change over time
	void StopHealthChangeOverTime();

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;
	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Health;

	// Health Max, Stamina, Strength
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float HealthMax;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cheats")
	bool bCheat_TakeNoDamage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cheats")
	bool bCheat_TakeAlmostNoDamage;
	

private:
	void HealthChangeOverTime_Tick(float Delta, float Duration);

	FTimerHandle HealthChangeTimerHandle;
	int32 HealthChangeTicksLeft;
};
