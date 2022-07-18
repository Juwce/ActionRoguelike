// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "TActionComponent.h"
#include "TAttributeComponent.h"
#include "TWorldUserWidget.h"
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

	ActionComp = CreateDefaultSubobject<UTActionComponent>("ActionComponent");

	// We want projectiles and other dynamic objects to hit the character mesh, not the capsule
	// GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	// Assigns AI controller to character when spawned (4.27 default is "Placed" only)
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	TargetActorBBKeyName = "TargetActor";
	
	OnDeathLifeSpanDuration = 10.f;
	
	HealthBarLocationComp = CreateDefaultSubobject<USceneComponent>("HealthBarLocation");
	HealthBarLocationComp->SetupAttachment(RootComponent);
	HealthBarLifetimeSeconds = 0.f;

	PlayerSpottedLocationComp = CreateDefaultSubobject<USceneComponent>("PlayerSpottedWidgetLocation");
	PlayerSpottedLocationComp->SetupAttachment(RootComponent);
	PlayerSpottedWidgetLifetimeSeconds = 3.f;
	
	TimeToHitMaterialParamName = "TimeToHit";
	HitFlashSpeedMaterialParamName = "HitFlashSpeed";
	HitFlashSpeed = 1.f;
}


void ATAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HasAuthority())
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &ATAICharacter::OnPawnSeen);
	}
	AttributeComp->OnHealthChanged.AddDynamic(this, &ATAICharacter::OnHealthChanged);
}


void ATAICharacter::SetBBTargetActor(AActor* Actor)
{
	if (ensure(HasAuthority()))
	{
		AAIController* AIController = Cast<AAIController>(GetController());
		if (ensure(AIController))
		{
			UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
			check(BBComp != nullptr);

			BBComp->SetValueAsObject(TargetActorBBKeyName, Actor);
		}
	}
}


AActor* ATAICharacter::GetBBTargetActor() const
{
	if (ensure(HasAuthority()))
	{
		AAIController* AIController = Cast<AAIController>(GetController());
		if (ensure(AIController))
		{
			const UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
			check(BBComp != nullptr);
			
			return Cast<AActor>(BBComp->GetValueAsObject(TargetActorBBKeyName));
		}
	}

	return nullptr;
}


void ATAICharacter::OnPawnSeen(APawn* Pawn)
{
	if (HasAuthority())
	{
		if (Pawn != GetBBTargetActor())
		{
			MulticastOnNewPawnSpotted();
		}
		
		SetBBTargetActor(Pawn);
	}
}

void ATAICharacter::MulticastOnNewPawnSpotted_Implementation()
{
	UTWorldUserWidget::ActivateWorldWidgetOnTimer(ActivePlayerSpottedWidget, PlayerSpottedWidgetClass,
		this,PlayerSpottedLocationComp->GetRelativeLocation(),
		PlayerSpottedWidgetLifetimeHandle, PlayerSpottedWidgetLifetimeSeconds);
}


void ATAICharacter::OnHealthChanged(AActor* InstigatorActor, UTAttributeComponent* OwningComp, float NewHealth,
                                    float Delta)
{
	if (Delta < 0.f)
	{
		TriggerHitFlashEffect();
		UTWorldUserWidget::ActivateWorldWidgetOnTimer(ActiveHealthBarWidget, HealthBarWidgetClass,
			this, HealthBarLocationComp->GetRelativeLocation(),
			HealthBarLifetimeHandle, HealthBarLifetimeSeconds);

		if (!OwningComp->IsAlive())
		{
			// TODO: replicate death
			Die();
		}
		else if (HasAuthority() && InstigatorActor != this)
		{
			SetBBTargetActor(InstigatorActor);
		}
	}
}


void ATAICharacter::Die()
{
	if (HasAuthority())
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
		
		// set lifespan (time to ragdoll and see corpse before destroying it)
		SetLifeSpan(OnDeathLifeSpanDuration);
	}

	// ragdoll - force all bones to use physics (bones can use physics or animation settings)
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();

	UTWorldUserWidget::DeactivateWorldWidgetAndTimer(ActiveHealthBarWidget, HealthBarLifetimeHandle);
	UTWorldUserWidget::DeactivateWorldWidgetAndTimer(ActivePlayerSpottedWidget, PlayerSpottedWidgetLifetimeHandle);
	
}


void ATAICharacter::TriggerHitFlashEffect()
{
	GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitMaterialParamName, GetWorld()->TimeSeconds);
	GetMesh()->SetScalarParameterValueOnMaterials(HitFlashSpeedMaterialParamName, HitFlashSpeed);
}
