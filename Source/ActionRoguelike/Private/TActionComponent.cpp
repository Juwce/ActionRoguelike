// Fill out your copyright notice in the Description page of Project Settings.


#include "TAction.h"
#include "TActionComponent.h"

// Sets default values for this component's properties
UTActionComponent::UTActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UTActionComponent::AddAction(const TSubclassOf<UTAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UTAction* NewAction = NewObject<UTAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
}

bool UTActionComponent::StartActionByName(const FName ActionName)
{
	for (UTAction* Action : Actions)
	{
		if (Action->ActionName == ActionName)
		{
			Action->StartAction(GetOwner());
			return true;
		}
	}
	return false;
}

bool UTActionComponent::StopActionByName(const FName ActionName)
{
	for (UTAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StopAction(GetOwner());
			return true;
		}
	}
	return false;
}
