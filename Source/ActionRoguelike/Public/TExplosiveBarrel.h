// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TExplosiveBarrel.generated.h"


class URadialForceComponent;
UCLASS()
class ACTIONROGUELIKE_API ATExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATExplosiveBarrel();

protected:
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	UFUNCTION(BlueprintCallable)
	void Explode();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ExplosionRetriggerDelaySeconds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ExplosionDamage;
	
	ERadialImpulseFalloff ImpulseFalloff;

public:	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(VisibleAnywhere)
	URadialForceComponent* RadialForceComp;

private:
	FTimerHandle ExplosionDelayTimerHandle;

	void EnableExplode();
	bool bCanExplode;
};
