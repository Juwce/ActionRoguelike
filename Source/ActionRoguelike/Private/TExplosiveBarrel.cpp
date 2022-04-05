// Fill out your copyright notice in the Description page of Project Settings.


#include "TExplosiveBarrel.h"

#include "TAttributeComponent.h"
#include "TCharacter.h"
#include "TProjectile_Magic.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ATExplosiveBarrel::ATExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	StaticMeshComp->SetSimulatePhysics(true);
	StaticMeshComp->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	SetRootComponent(StaticMeshComp);

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->SetupAttachment(RootComponent);

	// Optional, ignores 'Mass' of other objects (if false, the strength will need to be increased significantly to push
	// most objects, depending on their mass)
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->Radius = 750.f;
	RadialForceComp->ImpulseStrength = 2500.f;

	bCanExplode = true;
	ExplosionRetriggerDelaySeconds = 1.f;
	
	ExplosionDamage = -50.f;
}

void ATExplosiveBarrel::Explode()
{
	RadialForceComp->FireImpulse();
}

void ATExplosiveBarrel::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
                                  bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (bCanExplode)
	{
		Explode();

		// TODO: have the impulse apply the damage instead
		const ATCharacter* TCharacter = Cast<ATCharacter>(Other);
		if (TCharacter)
		{
			UTAttributeComponent* AttributeComp =Cast<UTAttributeComponent>(
				TCharacter->GetComponentByClass(UTAttributeComponent::StaticClass()));

			AttributeComp->ApplyHealthChange(this, ExplosionDamage);
		}

		bCanExplode = false;
		GetWorldTimerManager().SetTimer(
			ExplosionDelayTimerHandle, this, &ATExplosiveBarrel::EnableExplode, ExplosionRetriggerDelaySeconds, false);
		
	}
}

void ATExplosiveBarrel::EnableExplode()
{
	bCanExplode = true;
}
