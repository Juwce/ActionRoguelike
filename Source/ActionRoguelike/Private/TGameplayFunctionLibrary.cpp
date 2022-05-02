// Fill out your copyright notice in the Description page of Project Settings.


#include "TGameplayFunctionLibrary.h"

#include "TAttributeComponent.h"

bool UTGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, const float DamageAmount)
{
	UTAttributeComponent* TargetActorAttributes = UTAttributeComponent::GetAttributes(TargetActor);
	if (ensureMsgf(TargetActorAttributes, TEXT("Target actor is missing an attribute component. Cannot apply damage!")))
	{
		TargetActorAttributes->ApplyHealthChange(DamageCauser, DamageAmount);
		return true;
	}
	return false;
}

bool UTGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor,
	const float DamageAmount, const FHitResult& HitResult, const float ForceAmount)
{
	const bool bDamageApplied = ApplyDamage(DamageCauser, TargetActor, DamageAmount);
	if (!bDamageApplied)
	{
		return false;
	}

	UPrimitiveComponent* HitComp = HitResult.GetComponent();
	if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
	{
		// Direction = Target - Origin
		FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
		Direction.Normalize();
		
		HitComp->AddImpulseAtLocation(
			Direction * ForceAmount, HitResult.ImpactPoint, HitResult.BoneName);
	}

	return true;
}
