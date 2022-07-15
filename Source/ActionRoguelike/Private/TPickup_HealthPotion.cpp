// Fill out your copyright notice in the Description page of Project Settings.


#include "TPickup_HealthPotion.h"

#include "TCharacter.h"
#include "TAttributeComponent.h"
#include "Kismet/GameplayStatics.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

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

FText ATPickup_HealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	const UTAttributeComponent* AttributeComp = UTAttributeComponent::GetAttributes(InstigatorPawn);
	if (AttributeComp && AttributeComp->GetHealth() == AttributeComp->GetHealthMax())
	{
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health");
	}

	return FText::Format(LOCTEXT("HealthPotion_FullHealthWarning",
		"Costs {0} Credits. Restores {1} Health."), InteractionCreditCost, HealthGiven);
}

void ATPickup_HealthPotion::HealPawn(const APawn* Pawn)
{
	UTAttributeComponent* AttributeComp = UTAttributeComponent::GetAttributes(Pawn);
	if (AttributeComp)
	{
		 AttributeComp->ApplyHealthChange(this, HealthGiven);
	}
}

#undef LOCTEXT_NAMESPACE