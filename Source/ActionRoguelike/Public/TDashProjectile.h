// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TMagicProjectile.h"
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

	void Explode();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
