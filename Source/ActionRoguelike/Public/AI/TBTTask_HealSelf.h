// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealingType.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TBTTask_HealSelf.generated.h"

class UTAttributeComponent;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UTBTTask_HealSelf : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	
	EBTNodeResult::Type PerformHealSelf(UBehaviorTreeComponent& OwnerComp);

	UTAttributeComponent* GetOwnerAttributeComp(UBehaviorTreeComponent& OwnerComp) const;

	// How the heal value should be interpreted (percent of HealthMax, number of health points, etc.)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	HealingType HealValueType;

	// Amount to heal by: HealValueType determines how this value is interpreted (e.g. whether this value is a percent of HealthMax or number of health points to heal by)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	float HealValue;
	
	// How long it takes to heal the full amount (0 to instantly heal)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities", meta=(ClampMin = "0.0"))
	float HealDuration;
	
	// Total number of ticks that healing is split up by, evenly, over the heal duration
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities", meta=(ClampMin = "1"))
	int32 HealTicks;
};
