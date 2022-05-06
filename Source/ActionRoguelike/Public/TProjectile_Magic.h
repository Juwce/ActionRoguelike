// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TProjectileBase.h"
#include "TProjectile_Magic.generated.h"

class UTActionEffect;
class UProjectileMovementComponent;
class USphereComponent;
UCLASS()
class ACTIONROGUELIKE_API ATProjectile_Magic : public ATProjectileBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATProjectile_Magic();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	virtual void Explode_Implementation() override;

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float Damage;

	// ActionEffect to apply in addition to damage. Only applied if damage could be applied.
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TSubclassOf<UTActionEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag ParryTag;
	
	bool bHasBeenParried;

	/*
	 * Camera Shake
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> CameraShake;

	// Cameras inside this radius will be ignored
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraShake")
	float CameraShakeInnerRadius;

	// Cameras outside inner radius and inside this radius will be affected
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraShake")
	float CameraShakeOuterRadius;

	// Affects falloff of effect as it nears outer radius
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraShake")
	float CameraShakeFalloff;
};
