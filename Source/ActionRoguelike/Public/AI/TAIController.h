// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TAIController.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ATAIController : public AAIController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;
	
};
