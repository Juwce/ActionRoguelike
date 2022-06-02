// Fill out your copyright notice in the Description page of Project Settings.


#include "TAction.h"

#include "TActionComponent.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Net/UnrealNetwork.h"

UTAction::UTAction()
{
	bAutoStart = false;
	RepData.bIsRunning = false;
}

void UTAction::Initialize(UTActionComponent* OwningActionComponent)
{
	OwningActionComp = OwningActionComponent;
}

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
	UTActionComponent* Comp = GetOwningComponent();
	if (ensure(Comp))
	{
		Comp->ActiveGameplayTags.AppendTags(GrantsTags);
	}

	RepData.bIsRunning = true;
	LogOnScreen(this, FString::Printf(TEXT("Started : %s"), *ActionName.ToString()), FColor::Green);
}

void UTAction::StopAction_Implementation(AActor* InstigatorActor)
{
	UTActionComponent* Comp = GetOwningComponent();
	if (ensure(Comp))
	{
		Comp->ActiveGameplayTags.RemoveTags(GrantsTags);
	}

	RepData.bIsRunning = false;
	LogOnScreen(this, FString::Printf(TEXT("Stopping : %s"), *ActionName.ToString()), FColor::White);
}

UWorld* UTAction::GetWorld() const
{
	const AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}
	return nullptr;
}

UTActionComponent* UTAction::GetOwningComponent() const
{
	return OwningActionComp;
}

void UTAction::OnRep_IsRunning()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

void UTAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// default rule that says to unconditionally replicate the value to all clients when changed
	DOREPLIFETIME(UTAction, RepData);
	DOREPLIFETIME(UTAction, OwningActionComp);
}
