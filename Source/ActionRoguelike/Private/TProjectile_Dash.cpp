// Fill out your copyright notice in the Description page of Project Settings.


#include "TProjectile_Dash.h"

#include <queue>

#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ATProjectile_Dash::ATProjectile_Dash()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	DelayBetweenExplosionAndTeleport = 0.2f;
	DelayBetweenSpawnAndExplosion = 0.2f;
}

// Called when the game starts or when spawned
void ATProjectile_Dash::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);

	GetWorldTimerManager().SetTimer(
		TimerHandle, this, &ATProjectile_Dash::QueueTeleport, DelayBetweenSpawnAndExplosion, false);
}

// Triggered by parent class On Hit
void ATProjectile_Dash::Explode_Implementation()
{
	GetWorldTimerManager().ClearTimer(TimerHandle);
	QueueTeleport();
}

void ATProjectile_Dash::QueueTeleport()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
	EffectComp->DeactivateSystem();
	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);
	
	GetWorldTimerManager().SetTimer(
		TimerHandle, this, &ATProjectile_Dash::TeleportInstigator, DelayBetweenExplosionAndTeleport, false);
}

void ATProjectile_Dash::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();
	const FVector TeleportLocation = SphereComp->GetComponentLocation();
	const FRotator TeleportRotation = ActorToTeleport->GetActorRotation();

	if (ensure(ActorToTeleport))
	{
		GetInstigator()->TeleportTo(TeleportLocation, TeleportRotation, false, false);
	}
	
	Destroy();
}
