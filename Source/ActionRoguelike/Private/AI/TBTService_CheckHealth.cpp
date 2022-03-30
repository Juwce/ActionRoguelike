// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TBTService_CheckHealth.h"

#include "AIController.h"
#include "TAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UTBTService_CheckHealth::UTBTService_CheckHealth()
{
	LowHealthPercentThreshold = 20.f;
}

void UTBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UpdateHealthLowKey(OwnerComp);
}

void UTBTService_CheckHealth::UpdateHealthLowKey(UBehaviorTreeComponent& OwnerComp) const
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!ensure(BlackboardComp))
	{
		return;
	}

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!ensure(AIController))
	{
		return;
	}

	APawn* OwningPawn = AIController->GetPawn();
	if (!ensure(OwningPawn))
	{
		return;
	}

	UTAttributeComponent* AttributeComp =
		Cast<UTAttributeComponent>(OwningPawn->GetComponentByClass(UTAttributeComponent::StaticClass()));
	if (!ensureMsgf(AttributeComp,
					TEXT("Owning pawn must have a TAttributeComponent for this service to work. Please add one")))
	{
		return;
	}

	const float HealthPercent = AttributeComp->GetHealth() / AttributeComp->GetHealthMax();
	const bool bHealthAtOrBelowThreshold = (HealthPercent < LowHealthPercentThreshold ||
											FMath::IsNearlyEqual(HealthPercent, LowHealthPercentThreshold));
	BlackboardComp->SetValueAsBool(HealthLowKey.SelectedKeyName, bHealthAtOrBelowThreshold);
}
