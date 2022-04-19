// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TProjectileBase.h"
#include "TProjectile_Dash.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ATProjectile_Dash : public ATProjectileBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATProjectile_Dash();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Explode_Implementation() override;

	void QueueTeleport();
	void TeleportInstigator();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DelayBetweenSpawnAndExplosion;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DelayBetweenExplosionAndTeleport;

private:
	FTimerHandle TimerHandle;

};
