// Fill out your copyright notice in the Description page of Project Settings.


#include "TDashProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ATDashProjectile::ATDashProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ExplosionEffectComp = CreateDefaultSubobject<UParticleSystemComponent>("ExplosionEffectComp");
	ExplosionEffectComp->SetupAttachment(RootComponent);
	ExplosionEffectComp->SetAutoActivate(false);

	MovementComp->InitialSpeed = 3000.f;
	
	DelayBetweenExplosionAndTeleport = 0.2f;
	DelayBetweenSpawnAndExplosion = 0.2f;
}

// Called when the game starts or when spawned
void ATDashProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);

	GetWorldTimerManager().SetTimer(
		TimerHandle, this, &ATDashProjectile::QueueTeleport, DelayBetweenSpawnAndExplosion, false);
}

void ATDashProjectile::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (Other != GetInstigator())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
		QueueTeleport();
	}
}

void ATDashProjectile::QueueTeleport()
{
	ExplosionEffectComp->Activate();
	MovementComp->StopMovementImmediately();
	
	GetWorldTimerManager().SetTimer(
		TimerHandle, this, &ATDashProjectile::TeleportInstigator, DelayBetweenExplosionAndTeleport, false);
}

void ATDashProjectile::TeleportInstigator()
{
	const FVector TeleportLocation = SphereComp->GetComponentLocation();
	const FRotator TeleportRotation = GetInstigator()->GetActorRotation();
	
	GetInstigator()->TeleportTo(TeleportLocation, TeleportRotation);
	Destroy();
}

// Called every frame
void ATDashProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

