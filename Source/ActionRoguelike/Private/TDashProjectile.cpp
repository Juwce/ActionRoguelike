// Fill out your copyright notice in the Description page of Project Settings.


#include "TDashProjectile.h"

#include "Particles/ParticleSystemComponent.h"

// Sets default values
ATDashProjectile::ATDashProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ExplosionEffectComp = CreateDefaultSubobject<UParticleSystemComponent>("ExplosionEffectComp");
	ExplosionEffectComp->SetupAttachment(RootComponent);
	ExplosionEffectComp->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void ATDashProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ATDashProjectile::Explode()
{
}

// Called every frame
void ATDashProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

