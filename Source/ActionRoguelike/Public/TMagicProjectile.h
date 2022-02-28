// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TProjectileBase.h"
#include "TMagicProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
UCLASS()
class ACTIONROGUELIKE_API ATMagicProjectile : public ATProjectileBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATMagicProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Damage;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
