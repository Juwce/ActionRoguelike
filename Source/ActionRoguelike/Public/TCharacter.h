// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TCharacter.generated.h"

class ATProjectile;
class UTInteractionComponent;
class UCameraComponent;
class USpringArmComponent;

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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void PrimaryInteract();

	void PrimaryAttack();
	void SecondaryAttack();
	void TertiaryAttack();

	void Attack_StartTimer(const TSubclassOf<ATProjectile>& ProjectileClass);
	void Attack_TimeElapsed(const TSubclassOf<ATProjectile>& ProjectileClass);

	UFUNCTION(BlueprintCallable)
	bool ComputeAttackTarget(FVector& TargetLocation);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTInteractionComponent* InteractionComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* AttackAnim;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	TSubclassOf<ATProjectile> PrimaryProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	TSubclassOf<ATProjectile> SecondaryProjectileClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	TSubclassOf<ATProjectile> TertiaryProjectileClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxAttackTraceDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDrawDebugArrows;

private:
	FTimerHandle TimerHandle_Attack;

private:
	void DrawDebugArrows();

};
