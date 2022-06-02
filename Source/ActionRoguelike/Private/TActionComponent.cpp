// Fill out your copyright notice in the Description page of Project Settings.


#include "TActionComponent.h"
#include "TAction.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<bool> CVarDebugActiveGameplayTags(
	TEXT("ti.DebugActiveGameplayTags"),
	false,
	TEXT("Draw Active Gameplay Tags to the screen"),
	ECVF_Cheat);

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

	// server instantiates actions, then actions replicate automatically to clients
	if (GetOwner()->HasAuthority())
	{
		for (const TSubclassOf<UTAction>& ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}

void UTActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CVarDebugActiveGameplayTags.GetValueOnGameThread())
	{
		const FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, DebugMsg);
	}

	for (const UTAction* Action : Actions)
	{
		const FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;

		const FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s : IsRunning: %s : Outer: %s"),
		                                          *GetNameSafe(GetOwner()),
		                                          *Action->ActionName.ToString(),
		                                          Action->IsRunning() ?  TEXT("true") : TEXT("false"),
		                                          *GetNameSafe(Action->GetOuter()));

		LogOnScreen(this, ActionMsg, TextColor, 0.f);
	}
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
				// TODO: remove debug string or show conditionally via CVar
				const FString DebugMsg = FString::Printf(TEXT("Failed to run: %s (CanStart() is false.)"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, DebugMsg);
				continue;
			}

			// is client?
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

void UTActionComponent::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME( UTActionComponent, Actions );
}

bool UTActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	// Replicate every action that had a change
	for (UTAction* Action: Actions)
	{
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	// tells Unreal "there was a change in this component, please replicate my data"
	return WroteSomething;
}

