// Fill out your copyright notice in the Description page of Project Settings.


#include "TTargetDummy.h"

#include "TAttributeComponent.h"

// Sets default values
ATTargetDummy::ATTargetDummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<UTAttributeComponent>("AttributeComp");
}

// Called when the game starts or when spawned
void ATTargetDummy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATTargetDummy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	// Trigger when health is changed (damage/healing)
	AttributeComp->OnHealthChanged.AddDynamic(this, &ATTargetDummy::OnHealthChanged);
}

// Called every frame
void ATTargetDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATTargetDummy::OnHealthChanged(AActor* InstigatorActor, UTAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if (Delta < 0.f)
	{
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
}

