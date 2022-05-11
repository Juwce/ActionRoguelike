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

	UFUNCTION(BlueprintCallable)
	AActor* GetBBTargetActor() const;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UTAttributeComponent* OwningComp, float NewHealth, float Delta);
	
	void Die();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTActionComponent* ActionComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	FName TargetActorBBKeyName;
	
	// Lifespan of the character on death (Character is destroyed after this many seconds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float OnDeathLifeSpanDuration;

	/*
	 * Widgets
	 */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UTWorldUserWidget> HealthBarWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UTWorldUserWidget> PlayerSpottedWidgetClass;
	
	// Health bar will be placed at this component's location
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* HealthBarLocationComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	float HealthBarLifetimeSeconds;

	FTimerHandle HealthBarLifetimeHandle;
	
	UPROPERTY()
	UTWorldUserWidget* ActiveHealthBarWidget;
	
	// Player spotted widget will be placed at this component's location
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* PlayerSpottedLocationComp;
	
	UPROPERTY()
	UTWorldUserWidget* ActivePlayerSpottedWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	float PlayerSpottedWidgetLifetimeSeconds;

	FTimerHandle PlayerSpottedWidgetLifetimeHandle;

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
