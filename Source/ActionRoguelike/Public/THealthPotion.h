// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "THealthPotion.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ATHealthPotion : public AActor, public ITGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATHealthPotion();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ToggleActive();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HealthGiven;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	USoundBase* InteractSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	float InteractTimeout;

private:
	FTimerHandle TimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
