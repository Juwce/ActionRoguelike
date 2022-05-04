// Fill out your copyright notice in the Description page of Project Settings.


#include "TAction.h"

#include "TActionComponent.h"

bool UTAction::CanStart_Implementation(AActor* InstigatorActor)
{
	if (IsRunning())
	{
		return false;
	}
	
	const UTActionComponent* Comp = GetOwningComponent();
	if (ensure(Comp) && Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	
	return true;
}

void UTAction::StartAction_Implementation(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Log, TEXT("Starting TAction: %s"), *GetNameSafe(this));

	UTActionComponent* Comp = GetOwningComponent();
	if (ensure(Comp))
	{
		Comp->ActiveGameplayTags.AppendTags(GrantsTags);
	}

	bIsRunning = true;
}

void UTAction::StopAction_Implementation(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Log, TEXT("Stopping TAction: %s"), *GetNameSafe(this));

	ensureAlways(bIsRunning);
	
	UTActionComponent* Comp = GetOwningComponent();
	if (ensure(Comp))
	{
		Comp->ActiveGameplayTags.RemoveTags(GrantsTags);
	}

	bIsRunning = false;
}

UTActionComponent* UTAction::GetOwningComponent() const
{
	return Cast<UTActionComponent>(GetOuter());
}
