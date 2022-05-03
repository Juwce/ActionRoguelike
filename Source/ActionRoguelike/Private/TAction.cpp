// Fill out your copyright notice in the Description page of Project Settings.


#include "TAction.h"

#include "TActionComponent.h"

void UTAction::StartAction_Implementation(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Log, TEXT("Starting TAction: %s"), *GetNameSafe(this));

	UTActionComponent* Comp = GetOwningComponent();
	if (ensure(Comp))
	{
		Comp->ActiveGameplayTags.AppendTags(GrantsTags);
	}
}

void UTAction::StopAction_Implementation(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Log, TEXT("Stopping TAction: %s"), *GetNameSafe(this));
	
	UTActionComponent* Comp = GetOwningComponent();
	if (ensure(Comp))
	{
		Comp->ActiveGameplayTags.RemoveTags(GrantsTags);
	}
}

UTActionComponent* UTAction::GetOwningComponent() const
{
	return Cast<UTActionComponent>(GetOuter());
}