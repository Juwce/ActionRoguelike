// Fill out your copyright notice in the Description page of Project Settings.


#include "TActionEffect_Thorns.h"

#include "TActionComponent.h"
#include "TAttributeComponent.h"

UTActionEffect_Thorns::UTActionEffect_Thorns()
{
	ReflectedDamageRatio = 0.5f;
}

bool UTActionEffect_Thorns::CanStart_Implementation(AActor* InstigatorActor)
{
	return Super::CanStart_Implementation(InstigatorActor) &&
		UTAttributeComponent::GetAttributes(InstigatorActor) != nullptr;
}

void UTActionEffect_Thorns::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	UTAttributeComponent* Attributes = UTAttributeComponent::GetAttributes(InstigatorActor);
	if (!ensure(Attributes))
	{
		StopAction(InstigatorActor);
		return;
	}

	Attributes->OnHealthChanged.AddDynamic(this, &UTActionEffect_Thorns::OnHealthChanged);
}

void UTActionEffect_Thorns::StopAction_Implementation(AActor* InstigatorActor)
{
	Super::StopAction_Implementation(InstigatorActor);
	
	UTAttributeComponent* Attributes = UTAttributeComponent::GetAttributes(InstigatorActor);
	if (!ensure(Attributes))
	{
		StopAction(InstigatorActor);
		return;
	}
	
	Attributes->OnHealthChanged.RemoveDynamic(this, &UTActionEffect_Thorns::OnHealthChanged);
}

void UTActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, UTAttributeComponent* OwnerComp, float NewHealth,
	float Delta)
{
	if (!ensure(InstigatorActor) || !ensure(OwnerComp))
	{
		return;
	}
	
	// Ignore healing
	if (Delta >= 0.f)
	{
		return;
	}

	// Don't reflect self-damage
	AActor* DamagedActor = OwnerComp->GetOwner();
	if (InstigatorActor == DamagedActor)
	{
		return;
	}
	
	UTAttributeComponent* InstigatorAttributes = UTAttributeComponent::GetAttributes(InstigatorActor);
	if (ensure(InstigatorAttributes))
	{
		// Round negative .5 fraction down (e.g. we want -1.5 to round down to -2, not up to -1)
		const float ThornsDamage = FMath::Sign(Delta) * FMath::RoundToFloat(FMath::Abs(Delta) * ReflectedDamageRatio);
		InstigatorAttributes->ApplyHealthChange(DamagedActor, ThornsDamage);
	}
}
