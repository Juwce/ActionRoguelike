// Fill out your copyright notice in the Description page of Project Settings.


#include "TAction_ProjectileAttack.h"

#include "TCharacter.h"
#include "TProjectileBase.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

UTAction_ProjectileAttack::UTAction_ProjectileAttack()
{
	MaxAttackTraceDistance = 3000.f;
}

void UTAction_ProjectileAttack::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	OwnerCharacter = Cast<ATCharacter>(InstigatorActor);
	if (OwnerCharacter)
	{
		StartAttackTimer();
	}
}

void UTAction_ProjectileAttack::StopAction_Implementation(AActor* InstigatorActor)
{
	Super::StopAction_Implementation(InstigatorActor);

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UTAction_ProjectileAttack::StartAttackTimer()
{
	OwnerCharacter->PlayAnimMontage(AttackAnim);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTAction_ProjectileAttack::ExecuteAttack, .2f);
}

void UTAction_ProjectileAttack::ExecuteAttack() const
{
	if (ensure(ProjectileClass))
	{
		const FVector HandLocation = OwnerCharacter->GetMesh()->GetSocketLocation(OwnerCharacter->GetHandSocketName()); 

		FVector Target;
		ComputeAttackTarget(Target);

		const FRotator LookAtRotation = FRotationMatrix::MakeFromX(Target - HandLocation).Rotator();
		const FTransform SpawnTM = FTransform(LookAtRotation, HandLocation);

		FActorSpawnParameters SpawnParams;
		// ignore collision checks when spawning
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = OwnerCharacter;

		GetWorld()->SpawnActor<ATProjectileBase>(ProjectileClass, SpawnTM, SpawnParams);
		UGameplayStatics::SpawnEmitterAttached(
			SpellCastVFX, OwnerCharacter->GetMesh(), OwnerCharacter->GetHandSocketName());
	}
}

bool UTAction_ProjectileAttack::ComputeAttackTarget(FVector& TargetLocation) const
{
	const UCameraComponent* CameraComp = Cast<UCameraComponent>(
				OwnerCharacter->GetComponentByClass(UCameraComponent::StaticClass()));
	if (!ensure(CameraComp))
	{
		return false;
	}

	const FVector CameraLocation = CameraComp->GetComponentLocation();
	const FVector ControlRotation = OwnerCharacter->GetControlRotation().Vector();
	const FVector TraceEnd = CameraLocation + (ControlRotation * MaxAttackTraceDistance);
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(OwnerCharacter);
	
	FHitResult Hit;
	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(
		Hit, CameraLocation, TraceEnd, ObjectQueryParams, CollisionQueryParams);

	TargetLocation = bBlockingHit ? Hit.Location : TraceEnd;
	return bBlockingHit;
}
