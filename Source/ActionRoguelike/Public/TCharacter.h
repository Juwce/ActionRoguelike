// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TCharacter.generated.h"

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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void PrimaryAttack();
	void SecondaryAttack();

	void PrimaryAttack_TimeElapsed();
	void SecondaryAttack_TimeElapsed();

	void Attack_TimeElapsed(TSubclassOf<AActor> ProjectileClass);

	UFUNCTION(BlueprintCallable)
	bool ComputeAttackTarget(FVector& TargetLocation);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	TSubclassOf<AActor> PrimaryProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	TSubclassOf<AActor> SecondaryProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTInteractionComponent* InteractionComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDrawDebugArrows;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxAttackTraceDistance;

	FTimerHandle TimerHandle_Attack;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PrimaryInteract();
	
private:
	void DrawDebugArrows();

};
