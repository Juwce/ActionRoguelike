// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TAction.h"
#include "TAction_ProjectileAttack.generated.h"

class ATCharacter;
class ATProjectileBase;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UTAction_ProjectileAttack : public UTAction
{
	GENERATED_BODY()

public:
	UTAction_ProjectileAttack();
	
	bool ComputeAttackTarget(ATCharacter* InstigatorCharacter, FVector& TargetLocation) const;

private:
	virtual void StartAction_Implementation(AActor* InstigatorActor) override;

	virtual void StopAction_Implementation(AActor* InstigatorActor) override;
	
	void StartAttack(ATCharacter* InstigatorCharacter);

	UFUNCTION()
	void SpawnProjectile(ATCharacter* InstigatorCharacter);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TSubclassOf<ATProjectileBase> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float MaxAttackTraceDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	UParticleSystem* SpellCastVFX;

	// Duration between action starting and attack firing
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackDelaySeconds;

private:
	
	FTimerHandle TimerHandle;
};
