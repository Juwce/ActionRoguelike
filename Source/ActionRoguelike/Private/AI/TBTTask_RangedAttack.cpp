// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TBTTask_RangedAttack.h"

#include "AIController.h"
#include "TAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UTBTTask_RangedAttack::UTBTTask_RangedAttack()
{
	MaxBulletSpreadDegrees = 2.f;
}

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

	ACharacter* MyCharacter = Cast<ACharacter>(MyController->GetPawn());
	if (!MyCharacter)
	{
		return EBTNodeResult::Failed;
	}

	const AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
	if (!TargetActor)
	{
		return EBTNodeResult::Failed;
	}

	if (!UTAttributeComponent::IsActorAlive(TargetActor))
	{
		return EBTNodeResult::Failed;
	}

	const FVector MuzzleLocation = MyCharacter->GetMesh()->GetSocketLocation("Muzzle_01");
	const FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
	FRotator MuzzleRotation = Direction.Rotation();

	MuzzleRotation.Pitch += FMath::RandRange(0.f, MaxBulletSpreadDegrees);
	MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpreadDegrees, MaxBulletSpreadDegrees);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = MyCharacter;
	const AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);

	return NewProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
