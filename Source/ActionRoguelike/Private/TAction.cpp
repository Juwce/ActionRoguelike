// Fill out your copyright notice in the Description page of Project Settings.


#include "TAction.h"

void UTAction::StartAction_Implementation(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Log, TEXT("Starting TAction: %s"), *GetNameSafe(this));
}

void UTAction::StopAction_Implementation(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Log, TEXT("Stopping TAction: %s"), *GetNameSafe(this));
}
