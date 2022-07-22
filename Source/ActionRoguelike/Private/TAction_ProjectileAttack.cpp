// Fill out your copyright notice in the Description page of Project Settings.


#include "TAction_ProjectileAttack.h"

#include "TCharacter.h"
#include "TProjectileBase.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

UTAction_ProjectileAttack::UTAction_ProjectileAttack()
{
	MaxAttackTraceDistance = 3000.f;
	AttackDelaySeconds = 0.2f;
}

void UTAction_ProjectileAttack::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	ATCharacter* InstigatorCharacter = Cast<ATCharacter>(InstigatorActor);
	if (ensure(InstigatorCharacter))
	{
		StartAttack(InstigatorCharacter);
	}
	else
	{
		StopAction(InstigatorActor);
	}
}

void UTAction_ProjectileAttack::StopAction_Implementation(AActor* InstigatorActor)
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	
	Super::StopAction_Implementation(InstigatorActor);
}

void UTAction_ProjectileAttack::StartAttack(ATCharacter* InstigatorCharacter)
{
	InstigatorCharacter->PlayAnimMontage(AttackAnim);
	UGameplayStatics::SpawnEmitterAttached(
		SpellCastVFX, InstigatorCharacter->GetMesh(), InstigatorCharacter->GetHandSocketName());

	// server
	if (InstigatorCharacter->HasAuthority())
	{
		FTimerDelegate TimerDel;
		TimerDel.BindLambda([=]() { SpawnProjectile(InstigatorCharacter); });
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, AttackDelaySeconds, false);
	}
}

void UTAction_ProjectileAttack::SpawnProjectile(ATCharacter* InstigatorCharacter)
{
	if (ensure(ProjectileClass))
	{
			const FVector HandLocation =
				InstigatorCharacter->GetMesh()->GetSocketLocation(InstigatorCharacter->GetHandSocketName()); 

			FVector Target;
			ComputeAttackTarget(InstigatorCharacter, Target);

			const FRotator LookAtRotation = FRotationMatrix::MakeFromX(Target - HandLocation).Rotator();
			const FTransform SpawnTM = FTransform(LookAtRotation, HandLocation);

			FActorSpawnParameters SpawnParams;
			// ignore collision checks when spawning
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.Instigator = InstigatorCharacter;

			GetWorld()->SpawnActor<ATProjectileBase>(ProjectileClass, SpawnTM, SpawnParams);
	}

	StopAction(InstigatorCharacter);
}

bool UTAction_ProjectileAttack::ComputeAttackTarget(ATCharacter* InstigatorCharacter, FVector& TargetLocation) const
{
	const UCameraComponent* CameraComp = Cast<UCameraComponent>(
				InstigatorCharacter->GetComponentByClass(UCameraComponent::StaticClass()));
	if (!ensure(CameraComp))
	{
		return false;
	}

	// Trace directly down the camera's line of site (player's 'reticle')
	const FVector CameraLocation = CameraComp->GetComponentLocation();
	const FVector ControlRotation = InstigatorCharacter->GetControlRotation().Vector();
	const FVector TraceEnd = CameraLocation + (ControlRotation * MaxAttackTraceDistance);
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(InstigatorCharacter);
	
	FHitResult Hit;
	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(
		Hit, CameraLocation, TraceEnd, ObjectQueryParams, CollisionQueryParams);

	TargetLocation = bBlockingHit ? Hit.Location : TraceEnd;
	return bBlockingHit;
}
