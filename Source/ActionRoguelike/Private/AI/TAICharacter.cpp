// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TAICharacter.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "TAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ATAICharacter::ATAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	
	AttributeComp = CreateDefaultSubobject<UTAttributeComponent>("AttributeComponent");

	// Assigns AI controller to character when spawned (4.27 default is "Placed" only)
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ATAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ATAICharacter::OnPawnSeen);
	
	AttributeComp->OnHealthChanged.AddDynamic(this, &ATAICharacter::OnHealthChanged);
}

void ATAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
		check(BBComp != nullptr);

		BBComp->SetValueAsObject("TargetActor", Pawn);
	}
}

void ATAICharacter::OnHealthChanged(AActor* InstigatorActor, UTAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if (Delta < 0.f)
	{
		if (NewHealth <= 0.f)
		{
			// stop BT

			// ragdoll

			// set lifespan (time to ragdoll and see corpse before destroying it)
			SetLifeSpan(10.f);
		}
	}
}

