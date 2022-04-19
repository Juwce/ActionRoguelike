// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TCharacter.generated.h"

class ATProjectileBase;
class UCameraComponent;
class USpringArmComponent;
class UTAttributeComponent;
class UTInteractionComponent;

UCLASS()
class ACTIONROGUELIKE_API ATCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATCharacter();

protected:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*
	 * Movement
	 */
	void MoveForward(float Value);
	void MoveRight(float Value);

	/*
	 * Interaction
	 */
	void PrimaryInteract();
	
	/*
	 * Attacks
	 */
	void PrimaryAttack();
	void SecondaryAttack();
	void TertiaryAttack();

	void Attack_StartTimer(const TSubclassOf<ATProjectileBase>& ProjectileClass);
	void Attack_TimeElapsed(const TSubclassOf<ATProjectileBase>& ProjectileClass);

	UFUNCTION(BlueprintCallable)
	bool ComputeAttackTarget(FVector& TargetLocation);
	
	// Max distance away from the camera to trace for an attack target
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxAttackTraceDistance;

	/*
	 * Attributes
	 */
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UTAttributeComponent* OwningComp, float NewHealth, float Delta);

	/*
	 * Components
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTInteractionComponent* InteractionComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UParticleSystem* SpellCastVFX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat | Attack")
	UAnimMontage* AttackAnim;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat | Attack")
	TSubclassOf<ATProjectileBase> PrimaryProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat | Attack")
	TSubclassOf<ATProjectileBase> SecondaryProjectileClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat | Attack")
	TSubclassOf<ATProjectileBase> TertiaryProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UTAttributeComponent* AttributeComp;
	
	/*
	 * Hit Flash Effect
	 */
	void TriggerHitFlashEffect();
	
	// How quickly the flash upon being hit should happen
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat | HitFlash")
	float HitFlashSpeed;

	// Emissive color of the hit flash
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat | HitFlash")
	FColor HitFlashColor;

	// Material parameter name used to set the time to hit value
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	FName TimeToHitMaterialParamName;
	
	// Material parameter name used to set the hit flash speed value
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	FName HitFlashSpeedMaterialParamName;

	// Material parameter name used to set the hit flash color value
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	FName HitFlashColorMaterialParamName;

	/*
	 * Misc Properties
	 */
	// Whether or not to draw debug arrows for Controller & Actor forward vectors
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDrawDebugArrows;

	// Name of the hand socket on the character's skeletal mesh that various effects are spawned at the location of
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	FName HandSocketName;

	/*
	 * Console Commands / Cheats
	 */
	UFUNCTION(Exec)
	void CheatHealSelf(float Amount = 100);

	UFUNCTION(Exec)
	void CheatInvincible(bool Value = true);

	UFUNCTION(Exec)
	void CheatInvincibleAlmost(bool Value = true);
	
private:
	void DrawDebugArrows() const;

	FTimerHandle TimerHandle_Attack;

};
