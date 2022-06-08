// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, UTAttributeComponent*, OwningComp, float, NewHealth, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnRageChanged, AActor*, InstigatorActor, UTAttributeComponent*, OwningComp, float, NewRage, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UTAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTAttributeComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static UTAttributeComponent* GetAttributes(const AActor* FromActor);

public:
	/*
	 * Rage
	 */
	UFUNCTION(BlueprintCallable)
	void ApplyRageChange(AActor* InstigatorActor, float Delta);
	float GetRage() const { return Rage; }
	float GetRageMax() const { return RageMax; }

protected:

	UFUNCTION(BlueprintCallable)
	void ConvertHealthChangeToRage(AActor* InstigatorActor, UTAttributeComponent* OwningComp, float NewHealth, float HealthDelta);

	UPROPERTY(BlueprintAssignable)
	FOnRageChanged OnRageChanged;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRageChanged(AActor* InstigatorActor, float NewRage, float Delta);

	void SetRage(const float NewRage) { Rage = NewRage; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float Rage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float RageMax;

	// Lost health points will be converted to rage at this ratio (e.g. 10 lost health points with a conversion rate of
	// 2 would give 20 rage points. A conversion rate of 0.5 would give 5 rage points)
	UPROPERTY()
	float HealthToRageConversionRatio;

public:
	/*
	 * Health
	 */
	
	// Checks if the actor is alive. If the actor does not have an attribute component or is null, this returns false.
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static bool IsActorAlive(const AActor* Actor);
	
	UFUNCTION(BlueprintCallable)
	void ApplyHealthChange(AActor* InstigatorActor, float Delta);

	// Apply a health change over the given duration, split up evenly over the specified number of ticks
	UFUNCTION(BlueprintCallable)
	void ApplyHealthChangeOverTime(AActor* InstigatorActor, const float Delta, const float Duration, const int32 Ticks);

	// Immediately stop any ongoing health change over time
	UFUNCTION(BlueprintCallable)
	void StopHealthChangeOverTime();

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable)
	void Kill(AActor* InstigatorActor);
	
	float GetHealth() const { return Health; }
	float GetHealthMax() const { return HealthMax; }
	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cheats")
	bool bCheat_TakeNoDamage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cheats")
	bool bCheat_TakeAlmostNoDamage;
	
protected:
	
	void SetHealth(const float NewHealth) { Health = NewHealth; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float Health;

	// Health Max, Stamina, Strength
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float HealthMax;

	UFUNCTION(NetMulticast, Reliable) // TODO: mark as unreliable once we move the 'state' out of TCharacter
	void MulticastHealthChanged(AActor* Instigator, float NewHealth, float Delta);
	

private:
	void HealthChangeOverTime_Tick(AActor* InstigatorActor, const float Delta, const float Duration);

	FTimerHandle HealthChangeTimerHandle;
	int32 HealthChangeTicksLeft;
};
