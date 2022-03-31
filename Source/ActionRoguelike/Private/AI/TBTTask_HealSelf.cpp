// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TBTTask_HealSelf.h"

#include "AIController.h"
#include "TAttributeComponent.h"

EBTNodeResult::Type UTBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return PerformHealSelf(OwnerComp);
}

EBTNodeResult::Type UTBTTask_HealSelf::PerformHealSelf(UBehaviorTreeComponent& OwnerComp)
{
	if (!OwnerAttributeComp)
	{
		const bool Success = FindAndSetOwnerAttributeComp(OwnerComp);
		if (!Success)
		{
			return EBTNodeResult::Failed;
		}
	}

	const float HealAmount = OwnerAttributeComp->GetHealthMax() - OwnerAttributeComp->GetHealth();
	OwnerAttributeComp->ApplyHealthChange(HealAmount);

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
