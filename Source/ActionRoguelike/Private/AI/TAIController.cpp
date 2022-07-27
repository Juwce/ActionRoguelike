// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ATAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("BehaviorTree is nullptr! Please assign BehaviorTree in your AI controller.")))
	{
		RunBehaviorTree(BehaviorTree);
	}
}
