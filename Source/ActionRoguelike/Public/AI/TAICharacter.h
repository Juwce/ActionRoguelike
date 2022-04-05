// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "GameFramework/Character.h"
#include "TAICharacter.generated.h"

class UTAttributeComponent;
class UPawnSensingComponent;
UCLASS()
class ACTIONROGUELIKE_API ATAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATAICharacter();

protected:
	
	virtual void PostInitializeComponents() override;
	void SetBBTargetActor(AActor* Actor);

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);
	void Die();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UTAttributeComponent* OwningComp, float NewHealth, float Delta);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTAttributeComponent* AttributeComp;

	// Lifespan of the character on death (Character is destroyed after this many seconds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float OnDeathLifeSpanDuration;
};
