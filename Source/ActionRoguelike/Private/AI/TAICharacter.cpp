// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "TAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	
	OnDeathLifeSpanDuration = 10.f;
	
	TimeToHitMaterialParamName = "TimeToHit";
	HitFlashSpeedMaterialParamName = "HitFlashSpeed";
	HitFlashSpeed = 1.f;
}

void ATAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ATAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ATAICharacter::OnHealthChanged);
}

void ATAICharacter::SetBBTargetActor(AActor* Actor)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
		check(BBComp != nullptr);

		BBComp->SetValueAsObject("TargetActor", Actor);
	}
}

void ATAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetBBTargetActor(Pawn);
}

void ATAICharacter::Die()
{
	if (ensure(AttributeComp))
	{
		AttributeComp->StopHealthChangeOverTime();
	}
		
	// stop BT
	const AAIController* AIController = Cast<AAIController>(GetController());
	if (ensure(AIController))
	{
		AIController->GetBrainComponent()->StopLogic("Killed");
	}

	// ragdoll - force all bones to use physics (bones can use physics or animation settings)
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();

	// set lifespan (time to ragdoll and see corpse before destroying it)
	SetLifeSpan(OnDeathLifeSpanDuration);
}

void ATAICharacter::OnHealthChanged(AActor* InstigatorActor, UTAttributeComponent* OwningComp, float NewHealth,
                                    float Delta)
{
	if (Delta < 0.f)
	{
		TriggerHitFlashEffect();

		if (!OwningComp->IsAlive())
		{
			Die();
		}
		else if (InstigatorActor && InstigatorActor != this)
		{
			SetBBTargetActor(InstigatorActor);
		}
	}
}

void ATAICharacter::TriggerHitFlashEffect()
{
	GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitMaterialParamName, GetWorld()->TimeSeconds);
	GetMesh()->SetScalarParameterValueOnMaterials(HitFlashSpeedMaterialParamName, HitFlashSpeed);
}
