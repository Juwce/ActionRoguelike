// Fill out your copyright notice in the Description page of Project Settings.


#include "TPickup_HealthPotion.h"

#include "TCharacter.h"
#include "TAttributeComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATPickup_HealthPotion::ATPickup_HealthPotion()
{
	HealthGiven = 75.f;
}

bool ATPickup_HealthPotion::HealPawn(const APawn* Pawn)
{
	if (!ensureMsgf(Pawn, TEXT("Instigator pawn is null! Instigator should always be set when interacting.")))
	{
		return false;
	}

	UTAttributeComponent* AttributeComp = UTAttributeComponent::GetAttributes(Pawn);
	if (AttributeComp && AttributeComp->GetHealth() < AttributeComp->GetHealthMax())
	{
		 AttributeComp->ApplyHealthChange(Cast<AActor>(this), HealthGiven);
		 return true;
	}
	return false;
}

void ATPickup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	const bool HealSuccessful = HealPawn(InstigatorPawn);
	if (HealSuccessful)
	{
		 PlayInteractSound();
		 DeactivateAndCooldown();
	}
}
