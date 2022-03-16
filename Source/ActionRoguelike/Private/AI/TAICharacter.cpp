// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TAICharacter.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ATAICharacter::ATAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");

}

void ATAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ATAICharacter::OnPawnSeen);
}

void ATAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
		check(BBComp != nullptr);

		BBComp->SetValueAsObject("TargetActor", Pawn);

		DrawDebugString(GetWorld(), Pawn->GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
	}
}
