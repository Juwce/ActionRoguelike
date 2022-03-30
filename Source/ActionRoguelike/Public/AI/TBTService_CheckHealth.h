// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "TBTService_CheckHealth.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UTBTService_CheckHealth : public UBTService
{
	GENERATED_BODY()

public:
	UTBTService_CheckHealth();
	
private:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	void UpdateHealthLowKey(UBehaviorTreeComponent& OwnerComp) const;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector HealthLowKey;

	// Health is considered low if it falls to or below this percent of the owning character's Max Health
	UPROPERTY(EditAnywhere, Category = "AI")
	float LowHealthPercentThreshold;
};
