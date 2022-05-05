// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "GameFramework/Character.h"
#include "TAICharacter.generated.h"

class UTWorldUserWidget;
class UTAttributeComponent;
class UTActionComponent;
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

	UFUNCTION(BlueprintCallable)
	void ActivateHealthBarWidget();

	UFUNCTION(BlueprintCallable)
	void DeactivateHealthBarWidget();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTActionComponent* ActionComp;
	
	// Lifespan of the character on death (Character is destroyed after this many seconds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float OnDeathLifeSpanDuration;
	
	/*
	 * Health Bar Widget
	 */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	// Health bar will be placed at this component's location
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* HealthBarLocationComp;
	
	UPROPERTY()
	UTWorldUserWidget* ActiveHealthBarWidget;

	/*
	 * Hit Flash Effect
	 * TODO: Everything below is duplicated from the TCharacter class and could be moved to a base class or component
	 */
	void TriggerHitFlashEffect();
	
	// How quickly the flash upon being hit should happen
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat | HitFlash")
	float HitFlashSpeed;

	// Material parameter name used to set the time to hit value
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	FName TimeToHitMaterialParamName;
	
	// Material parameter name used to set the hit flash speed value
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	FName HitFlashSpeedMaterialParamName;
};