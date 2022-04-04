// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TBTTask_HealSelf.h"

#include "AIController.h"
#include "TAttributeComponent.h"
#include "BehaviorTree/Tasks/BTTask_FinishWithResult.h"

EBTNodeResult::Type UTBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!OwnerAttributeComp)
	{
		const bool Success = FindAndSetOwnerAttributeComp(OwnerComp);
		if (!Success)
		{
			return EBTNodeResult::Failed;
		}
	}
	
	return PerformHealSelf(OwnerComp);
}

EBTNodeResult::Type UTBTTask_HealSelf::PerformHealSelf(UBehaviorTreeComponent& OwnerComp)
{
	float TotalPointsToHeal = HealValueType == HealingType::HealthPoints;
	
	if (HealValueType == HealingType::HealthPoints)
	{
		TotalPointsToHeal = HealValue;
	}
	else if (HealValueType == HealingType::PercentOfHealthMax)
	{
		TotalPointsToHeal = HealValue * OwnerAttributeComp->GetHealthMax();
	}
	else
	{
		ensureMsgf(false, TEXT("Selected healing type not implemented! Ask a developer to add support."));
		return EBTNodeResult::Failed;
	}

	OwnerAttributeComp->ApplyHealthChangeOverTime(TotalPointsToHeal, HealDuration, HealTicks);
	return EBTNodeResult::Succeeded;
}

bool UTBTTask_HealSelf::FindAndSetOwnerAttributeComp(UBehaviorTreeComponent& OwnerComp)
{
	const AAIController* MyController = OwnerComp.GetAIOwner();
	if (!ensure(MyController))
	{
		return false;
	}

	const APawn* MyPawn = Cast<APawn>(MyController->GetPawn());
	if (!ensure(MyPawn))
	{
		return false;
	}

	UTAttributeComponent* AttributeComp =
		Cast<UTAttributeComponent>(MyPawn->GetComponentByClass(UTAttributeComponent::StaticClass()));

	if (!ensureMsgf(AttributeComp,
				    TEXT("Owning pawn must have an Attribute Component to be able to heal. Please add one.")))
	{
		return false;
	}

	OwnerAttributeComp = AttributeComp;
	return true;
}
