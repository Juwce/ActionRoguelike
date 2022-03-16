// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ATAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);

	// // TODO: Not a good way to get the player (prof mentioned pawn sensing), but fine for prototyping
	// APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	// if (MyPawn)
	// {
	// 	// TODO: Want to expose "MoveToLocation" as a variable... bad to have it hard coded
	// 	GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());
	// 	GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	// }
	
}
