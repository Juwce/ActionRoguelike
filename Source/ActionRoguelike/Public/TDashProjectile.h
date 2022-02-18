// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TProjectile.h"
#include "TDashProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ATDashProjectile : public ATProjectile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATDashProjectile();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* ExplosionEffectComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

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
