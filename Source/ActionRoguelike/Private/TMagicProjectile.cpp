// Fill out your copyright notice in the Description page of Project Settings.


#include "TMagicProjectile.h"

#include "TAttributeComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ATMagicProjectile::ATMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Damage = -20.f;
}

// Called when the game starts or when spawned
void ATMagicProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}

void ATMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ATMagicProjectile::OnActorOverlap);
}

void ATMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		UTAttributeComponent* AttributeComp = Cast<UTAttributeComponent>(
			OtherActor->GetComponentByClass(UTAttributeComponent::StaticClass())
		);
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(Damage);
			Destroy();
		}
	}
}

// Called every frame
void ATMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

