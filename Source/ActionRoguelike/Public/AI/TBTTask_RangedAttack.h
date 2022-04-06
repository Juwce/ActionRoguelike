// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TBTTask_RangedAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UTBTTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()

	UTBTTask_RangedAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	// Attack Target Actor, fails if no Target Actor or Target Actor is not alive
	EBTNodeResult::Type PerformRangedAttack(UBehaviorTreeComponent& OwnerComp);

	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<AActor> ProjectileClass;

	// Maximum deviation from Target the attack will fire in, in degrees. Will never deviate towards the ground,
	// only in the up/left/right direction(s).
	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxBulletSpreadDegrees;
};
