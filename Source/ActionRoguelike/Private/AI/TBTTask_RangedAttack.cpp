// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TBTTask_RangedAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UTBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return PerformRangedAttack(OwnerComp);
}

EBTNodeResult::Type UTBTTask_RangedAttack::PerformRangedAttack(UBehaviorTreeComponent& OwnerComp)
{
	const AAIController* MyController = OwnerComp.GetAIOwner();
	if (!ensure(MyController))
	{
		return EBTNodeResult::Failed;
	}

	const ACharacter* MyCharacter = Cast<ACharacter>(MyController->GetPawn());
	if (!MyCharacter)
	{
		return EBTNodeResult::Failed;
	}

	const FVector MuzzleLocation = MyCharacter->GetMesh()->GetSocketLocation("Muzzle_01");
	const AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
	if (!TargetActor)
	{
		return EBTNodeResult::Failed;
	}

	const FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
	const FRotator MuzzleRotation = Direction.Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	const AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);

	return NewProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
