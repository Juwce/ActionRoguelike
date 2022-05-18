// Fill out your copyright notice in the Description page of Project Settings.


#include "TActionComponent.h"
#include "TAction.h"

// Sets default values for this component's properties
UTActionComponent::UTActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	// TODO: Toggle on/of with debug actor gameplay tags console flag
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void UTActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (const TSubclassOf<UTAction>& ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
}

void UTActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// TODO: Make toggleable with a debug actor gameplay tags console flag
	const FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, DebugMsg);
}

UTActionComponent* UTActionComponent::GetActionComponent(const AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UTActionComponent>(FromActor->GetComponentByClass(UTActionComponent::StaticClass()));
	}
	return nullptr;
}

void UTActionComponent::AddAction(AActor* Instigator, const TSubclassOf<UTAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UTAction* NewAction = NewObject<UTAction>(GetOwner(), ActionClass);
	if (ensure(NewAction))
	{
		NewAction->Initialize(this);
		if (NewAction->bAutoStart)
		{
			NewAction->StartAction(Instigator);
		}
		
		Actions.Add(NewAction);
	}
}

void UTActionComponent::RemoveAction(UTAction* Action)
{
	if (Action && ensureMsgf(!Action->IsRunning(), TEXT("Attempting to remove a running action! Action will not be"
													    " removed.")))
	{
		Actions.Remove(Action);
	}
}

bool UTActionComponent::HasAction(const TSubclassOf<UTAction> ActionClass) const
{
	for (const UTAction* Action : Actions)
	{
		if (Action && Action->GetClass() == ActionClass)
		{
			return true;
		}
	}

	return false;
}

bool UTActionComponent::StartActionByName(AActor* Instigator, const FName ActionName)
{
	for (UTAction* Action : Actions)
	{
		if (Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				// TODO: remove debug string
				const FString DebugMsg = FString::Printf(TEXT("Failed to run: %s (CanStart() is false.)"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, DebugMsg);
				continue;
			}

			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}
			
			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

bool UTActionComponent::StopActionByName(AActor* Instigator, const FName ActionName)
{
	for (UTAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}
	return false;
}

void UTActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void UTActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}
