// Fill out your copyright notice in the Description page of Project Settings.


#include "TPickupActor.h"

#include "TCharacter.h"
#include "TPlayerController.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<bool> CVarDebugPickupActorReplication(
	TEXT("ti.DebugPickupActorReplication"),
	false,
	TEXT("Draw added actions to the screen"),
	ECVF_Cheat);

// Sets default values
ATPickupActor::ATPickupActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(StaticMesh);
	StaticMesh->SetCollisionProfileName("OverlapAllDynamic");

	CooldownDuration = 10.f;
	bActive = true;

	bReplicates = true;
}

void ATPickupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensureMsgf(HasAuthority(), TEXT("Client attempting to interact with PickupActor. Interaction should only be"
		" executed on the server! To avoid this use InteractionComponent's ServerInteract() RPC.")))
	{
		return;
	}
	
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	if (CanPickup(InstigatorPawn))
	{
		DoPickup(InstigatorPawn);
	}
}

FText ATPickupActor::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
}

bool ATPickupActor::CanPickup(const APawn* InstigatorPawn)
{
	const ATPlayerController* InstigatorPlayer = Cast<ATPlayerController>(InstigatorPawn->GetController());
	if (InstigatorPlayer)
	{
		return InstigatorPlayer->HasEnoughCredits(InteractionCreditCost);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Interacting actor %s does not implement credits. Credit cost / reward for"
								  " interacting with this pickup will not apply."), *GetNameSafe(InstigatorPawn));
	return false;
}

void ATPickupActor::DoPickup(APawn* InstigatorPawn)
{
	bActive = false;
	OnRep_bActive();
	
	ApplyInteractionCredits(InstigatorPawn);
	GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &ATPickupActor::Reactivate, CooldownDuration, false);
}

void ATPickupActor::Reactivate()
{
	bActive = true;
	OnRep_bActive();
}

void ATPickupActor::OnRep_bActive()
{
	if (bActive)
	{
		SetActorHiddenInGame(false);
		SetActorEnableCollision(true);
	}
	else
	{
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		PlayInteractSound();
	}
	
	if (CVarDebugPickupActorReplication.GetValueOnGameThread())
	{
		LogOnScreen(GetWorld(),
			FString::Printf(TEXT("[%s] %s"),
				*GetNameSafe(this),
				bActive ? TEXT("Activated") : TEXT("Deactivated")));
	}
}

void ATPickupActor::PlayInteractSound() const
{
	if (InteractSound)
	{
		 UGameplayStatics::PlaySoundAtLocation(GetWorld(), InteractSound, GetActorLocation());
	}
}

void ATPickupActor::ApplyInteractionCredits(APawn* InstigatorPawn)
{
	const float CreditDelta = -InteractionCreditCost + InteractionCreditReward;
	if (!FMath::IsNearlyZero(CreditDelta))
	{
		ATPlayerController* InstigatorPlayer = Cast<ATPlayerController>(InstigatorPawn->GetController());
		if (InstigatorPlayer)
		{
			InstigatorPlayer->ApplyCreditChange(this, CreditDelta);
		}
	}
}

void ATPickupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(ATPickupActor, bActive);
}