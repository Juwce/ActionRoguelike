// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UTBTService_CheckAttackRange::UTBTService_CheckAttackRange()
{
	AttackRange = 2000.f;
}

void UTBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UpdateWithinRangeKey(OwnerComp);
}

void UTBTService_CheckAttackRange::UpdateWithinRangeKey(UBehaviorTreeComponent& OwnerComp) const
{
	// Check distance between AI pawn and TargetActor
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!ensure(BlackboardComp))
	{
		return;
	}
		
	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
	if (!TargetActor)
	{
		return;
	}
		
	AAIController* TargetController = OwnerComp.GetAIOwner();
	if (!ensure(TargetController))
	{
		return;
	}
		
	APawn* AIPawn = TargetController->GetPawn();
	if (!ensure(AIPawn))
	{
		return;
	}

	const float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
	const bool bWithinRange = DistanceTo <= AttackRange;
	bool bHasLineOfSight;
	if (!bWithinRange)
	{
		bHasLineOfSight = false;
	}
	else
	{
		bHasLineOfSight = TargetController->LineOfSightTo(TargetActor);
	}
	
	BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, bWithinRange && bHasLineOfSight);
}
