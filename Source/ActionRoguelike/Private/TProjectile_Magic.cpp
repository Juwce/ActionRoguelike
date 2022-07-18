// Fill out your copyright notice in the Description page of Project Settings.


#include "TProjectile_Magic.h"

#include "TActionComponent.h"
#include "TAttributeComponent.h"
#include "TGameplayFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATProjectile_Magic::ATProjectile_Magic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InitialLifeSpan = 10.f;
	Damage = -20.f;
	bHasBeenParried = false;
	bEnsureInstigator = true;
}

// Called when the game starts or when spawned
void ATProjectile_Magic::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}

void ATProjectile_Magic::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ATProjectile_Magic::OnActorOverlap);
}

void ATProjectile_Magic::Explode_Implementation()
{
	Super::Explode_Implementation();

	if (ensure(CameraShake))
	{
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShake, GetActorLocation(),
			CameraShakeInnerRadius, CameraShakeOuterRadius, CameraShakeFalloff);
	}
}

void ATProjectile_Magic::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult& SweepResult)
{
	// Ignore Collision with instigators
	APawn* InstigatorPawn = GetInstigator();
	if (!OtherActor || OtherActor == InstigatorPawn)
	{
		return;
	}
	
	UTActionComponent* ActionComp = UTActionComponent::GetActionComponent(OtherActor);

	// Parry
	if (!bHasBeenParried)
	{
		if (!bHasBeenParried && ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			MovementComp->Velocity *= -1.f;
			SetInstigator(Cast<APawn>(OtherActor));
			bHasBeenParried = true;
			return;
		}
	}

	// Deal damage
	if (UTAttributeComponent::GetAttributes(OtherActor))
	{
		const bool bDamageApplied = UTGameplayFunctionLibrary::ApplyDirectionalDamage(
			InstigatorPawn, OtherActor, Damage, SweepResult);
		if (bDamageApplied)
		{
			Explode();

			if (HasAuthority() && ActionComp && DamageEffectClass)
			{
				ActionComp->AddAction(GetInstigator(), DamageEffectClass);
			}
		}
	}
}
