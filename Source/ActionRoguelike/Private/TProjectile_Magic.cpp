// Fill out your copyright notice in the Description page of Project Settings.


#include "TProjectile_Magic.h"

#include "TAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATProjectile_Magic::ATProjectile_Magic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Damage = -20.f;
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
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShake, GetActorLocation(), CameraShakeInnerRadius, CameraShakeOuterRadius, CameraShakeFalloff);
	}
}

void ATProjectile_Magic::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* InstigatorPawn = GetInstigator();
	if (OtherActor && OtherActor != InstigatorPawn)
	{
		UTAttributeComponent* AttributeComp = UTAttributeComponent::GetAttributes(OtherActor);
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(InstigatorPawn, Damage);
			Explode();
		}
	}
}
