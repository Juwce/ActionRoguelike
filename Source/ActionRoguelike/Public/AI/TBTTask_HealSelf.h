// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	bool FindAndSetOwnerAttributeComp(UBehaviorTreeComponent& OwnerComp);
	
	UPROPERTY()
	UTAttributeComponent* OwnerAttributeComp = nullptr;
	
	// // Total amount to heal by
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	// float HealAmount;
	//
	// // How long it takes to heal the full amount (0 for instant healing)
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	// float HealDuration;
	//
	// // The number of times per second to apply healing
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	// float HealTickRate;
};
