// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TProjectileBase.h"
#include "TDashProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ATDashProjectile : public ATProjectileBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATDashProjectile();

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
