// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TInteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UTInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	void PrimaryInteract();

	// Sets default values for this component's properties
	UTInteractionComponent();

protected:
	UFUNCTION(BlueprintCallable)
	bool ComputeInteractCandidates(TArray<FHitResult>& Hits);
	
	UPROPERTY(EditAnywhere)
	float MaxInteractDistance;
		
};
