// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TProjectile.generated.h"


class UProjectileMovementComponent;
class USphereComponent;
UCLASS()
class ACTIONROGUELIKE_API ATProjectile : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ATProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* EffectComp;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
