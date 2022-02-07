// Fill out your copyright notice in the Description page of Project Settings.


#include "TExplosiveBarrel.h"

#include "TMagicProjectile.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ATExplosiveBarrel::ATExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	StaticMeshComp->SetSimulatePhysics(true);
	StaticMeshComp->SetCollisionProfileName("PhysicsActor");
	SetRootComponent(StaticMeshComp);

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void ATExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATExplosiveBarrel::Explode()
{
	const FVector Origin = StaticMeshComp->GetComponentLocation();
	RadialForceComp->FireImpulse();
}

void ATExplosiveBarrel::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
                                  bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	Explode();
	UE_LOG(LogTemp, Warning, TEXT("Boom %s"), *Other->GetName());
}

// Called every frame
void ATExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

