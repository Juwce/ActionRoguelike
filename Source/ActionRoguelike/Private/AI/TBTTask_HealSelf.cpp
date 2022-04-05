// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TBTTask_HealSelf.h"

#include "AIController.h"
#include "TAttributeComponent.h"
#include "BehaviorTree/Tasks/BTTask_FinishWithResult.h"

EBTNodeResult::Type UTBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return PerformHealSelf(OwnerComp);
}

EBTNodeResult::Type UTBTTask_HealSelf::PerformHealSelf(UBehaviorTreeComponent& OwnerComp)
{
	UTAttributeComponent* OwnerAttributeComp = GetOwnerAttributeComp(OwnerComp);
	if (!ensureMsgf(OwnerAttributeComp,
				    TEXT("Owning pawn must have an Attribute Component to be able to heal. Please add one.")))
	{
		return EBTNodeResult::Failed;
	}
	
	float TotalPointsToHeal;
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

	OwnerAttributeComp->ApplyHealthChangeOverTime(OwnerAttributeComp->GetOwner(), TotalPointsToHeal, HealDuration, HealTicks);
	return EBTNodeResult::Succeeded;
}

UTAttributeComponent* UTBTTask_HealSelf::GetOwnerAttributeComp(UBehaviorTreeComponent& OwnerComp) const
{
	const AAIController* MyController = OwnerComp.GetAIOwner();
	if (!ensure(MyController))
	{
		return nullptr;
	}

	const APawn* MyPawn = Cast<APawn>(MyController->GetPawn());
	if (!ensure(MyPawn))
	{
		return nullptr;
	}

	UTAttributeComponent* AttributeComp = UTAttributeComponent::GetAttributes(MyPawn);

	return AttributeComp;
}
