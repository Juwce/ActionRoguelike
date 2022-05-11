// Fill out your copyright notice in the Description page of Project Settings.


#include "TPickup_GrantActions.h"

#include "TActionComponent.h"

bool ATPickup_GrantActions::CanPickup(const APawn* InstigatorPawn)
{
	const UTActionComponent* ActionComp = UTActionComponent::GetActionComponent(InstigatorPawn);
	if (!ActionComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Interacting actor %s does not have an action component. Cannot interact with "
									  "(%s)"), *GetNameSafe(InstigatorPawn), *GetNameSafe(this));
		return false;
	}
	
	for (const TSubclassOf<UTAction> ActionClass : GrantedActions)
	{
		if (!ActionComp->HasAction(ActionClass))
		{
			return true;
		}
	}

	return false;
}

void ATPickup_GrantActions::DoPickup(APawn* InstigatorPawn)
{
	Super::DoPickup(InstigatorPawn);

	UTActionComponent* ActionComp = UTActionComponent::GetActionComponent(InstigatorPawn);
	if (ActionComp)
	{
		for (const TSubclassOf<UTAction> ActionClass : GrantedActions)
		{
			ActionComp->AddAction(this, ActionClass);
		}
	}
}
