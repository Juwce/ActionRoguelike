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

bool ATPickup_HealthPotion::CanPickup(const APawn* InstigatorPawn)
{
	if (Super::CanPickup(InstigatorPawn))
	{
		// Only heal if not at max health
		const UTAttributeComponent* AttributeComp = UTAttributeComponent::GetAttributes(InstigatorPawn);
		return AttributeComp && AttributeComp->GetHealth() < AttributeComp->GetHealthMax();
	}
	
	return false;
}

void ATPickup_HealthPotion::DoPickup(APawn* InstigatorPawn)
{
	Super::DoPickup(InstigatorPawn);
	HealPawn(InstigatorPawn);
}

void ATPickup_HealthPotion::HealPawn(const APawn* Pawn)
{
	UTAttributeComponent* AttributeComp = UTAttributeComponent::GetAttributes(Pawn);
	if (AttributeComp)
	{
		 AttributeComp->ApplyHealthChange(this, HealthGiven);
	}
}