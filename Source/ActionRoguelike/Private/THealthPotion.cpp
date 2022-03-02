// Fill out your copyright notice in the Description page of Project Settings.


#include "THealthPotion.h"

#include "TCharacter.h"
#include "TAttributeComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATHealthPotion::ATHealthPotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(StaticMesh);
	StaticMesh->SetCollisionProfileName("ECC_WorldDynamic");

	InteractTimeout = 10.f;
	HealthGiven = 75.f;
}

void ATHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (ensure(InstigatorPawn))
	{
		UTAttributeComponent* AttributeComp = Cast<UTAttributeComponent>(InstigatorPawn->GetComponentByClass(UTAttributeComponent::StaticClass()));
		if (AttributeComp && AttributeComp->GetHealth() < AttributeComp->GetHealthMax())
		{
			if (InteractSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), InteractSound, GetActorLocation());
			}
			AttributeComp->ApplyHealthChange(HealthGiven);
			ToggleActive();
			GetWorldTimerManager().SetTimer(TimerHandle, this, &ATHealthPotion::ToggleActive, InteractTimeout, false);
		}
	}
}

// Called when the game starts or when spawned
void ATHealthPotion::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATHealthPotion::ToggleActive()
{
	SetActorEnableCollision(!GetActorEnableCollision());
	SetActorHiddenInGame(!IsHidden());
}

// Called every frame
void ATHealthPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

