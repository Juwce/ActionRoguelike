// Fill out your copyright notice in the Description page of Project Settings.


#include "TPickupActor.h"

#include "TCharacter.h"
#include "TPlayerController.h"
#include "TPlayerState.h"
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
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	if (CanPickup(InstigatorPawn))
	{
		DoPickup(InstigatorPawn);
	}
}

bool ATPickupActor::CanPickup(const APawn* InstigatorPawn)
{
	const ATPlayerController* InstigatorPlayer = Cast<ATPlayerController>(InstigatorPawn->GetController());
	if (InstigatorPlayer)
	{
		return InstigatorPlayer->HasEnoughCredits(InteractionCreditCost);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Interacting actor %s does not implement credits. Credit cost / reward for"
								  " interacting with this pickup will not apply."), *GetNameSafe(InstigatorPawn));
	return false;
}

void ATPickupActor::DoPickup(APawn* InstigatorPawn)
{
	PlayInteractSound();
	ApplyInteractionCredits(InstigatorPawn);
	DeactivateAndCooldown();
}

void ATPickupActor::DeactivateAndCooldown()
{
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &ATPickupActor::Activate, CooldownDuration, false);
}

void ATPickupActor::Activate()
{
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
}

void ATPickupActor::PlayInteractSound() const
{
	if (InteractSound)
	{
		 UGameplayStatics::PlaySoundAtLocation(GetWorld(), InteractSound, GetActorLocation());
	}
}

void ATPickupActor::ApplyInteractionCredits(APawn* InstigatorPawn)
{
	ATPlayerController* InstigatorPlayer = Cast<ATPlayerController>(InstigatorPawn->GetController());
	if (InstigatorPlayer)
	{
		InstigatorPlayer->ApplyCreditChange(-InteractionCreditCost + InteractionCreditReward);
	}
}
