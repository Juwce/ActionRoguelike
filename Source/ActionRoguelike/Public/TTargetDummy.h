// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TTargetDummy.generated.h"

class UTAttributeComponent;
UCLASS()
class ACTIONROGUELIKE_API ATTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATTargetDummy();

protected:

	virtual void PostInitializeComponents() override;

public:	

	UPROPERTY(VisibleAnywhere)
	UTAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UTAttributeComponent* OwningComp, float NewHealth, float Delta);
};
