// Fill out your copyright notice in the Description page of Project Settings.


#include "TActionComponent.h"
#include "TAction.h"

// Sets default values for this component's properties
UTActionComponent::UTActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UTActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (const TSubclassOf<UTAction>& ActionClass : DefaultActions)
	{
		AddAction(ActionClass);
	}
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

bool UTActionComponent::StartActionByName(AActor* Instigator, const FName ActionName)
{
	for (UTAction* Action : Actions)
	{
		if (Action->ActionName == ActionName)
		{
			Action->StartAction(Instigator);
			return true;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("StartActionByName: Action by the name \"%s\" not found on Actor %s"),
		*ActionName.ToString(), *GetNameSafe(GetOwner()));
	return false;
}

bool UTActionComponent::StopActionByName(AActor* Instigator, const FName ActionName)
{
	for (UTAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StopAction(Instigator);
			return true;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("StopActionByName: Action by the name \"%s\" not found on Actor %s"),
		*ActionName.ToString(), *GetNameSafe(GetOwner()));
	return false;
}
