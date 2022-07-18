// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TProjectileBase.generated.h"


class UProjectileMovementComponent;
class USphereComponent;
UCLASS(ABSTRACT) // 'ABSTRACT' marks this class as incomplete, keeping this out of certain dropdown windows like SpawnActor in Unreal Editor
class ACTIONROGUELIKE_API ATProjectileBase : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ATProjectileBase();

protected:

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components");
	UProjectileMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components");
	UParticleSystemComponent* EffectComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* AudioComp;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* ImpactVFX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundBase* ImpactSound;
	
	// If true, ensures that instigator is set when the projectile is spawned. Disable if not being spawned by a pawn.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bEnsureInstigator;
	
};
