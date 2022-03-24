// Fill out your copyright notice in the Description page of Project Settings.


#include "TPickupActor.h"

#include "TCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATPickupActor::ATPickupActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(StaticMesh);
	StaticMesh->SetCollisionProfileName("ECC_WorldDynamic");

	CooldownDuration = 10.f;
}

void ATPickupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	if (ensure(InstigatorPawn))
	{
		PlayInteractSound();
		DeactivateAndCooldown();
	}
}

void ATPickupActor::DeactivateAndCooldown()
{
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &ATPickupActor::Activate, CooldownDuration, false);
}

void ATPickupActor::PlayInteractSound()
{
	if (InteractSound)
	{
		 UGameplayStatics::PlaySoundAtLocation(GetWorld(), InteractSound, GetActorLocation());
	}
}

void ATPickupActor::Activate()
{
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
}
