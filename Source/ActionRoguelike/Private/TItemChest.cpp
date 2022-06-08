// Fill out your copyright notice in the Description page of Project Settings.


#include "TItemChest.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ATItemChest::ATItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	SetRootComponent(BaseMesh);
	
	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110;

	bReplicates = true;
}


void ATItemChest::OnRep_bLidOpened()
{
	const float CurrPitch = bLidOpened ? TargetPitch : 0.f;
	LidMesh->SetRelativeRotation(FRotator(CurrPitch, 0, 0));
}


void ATItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpened = !bLidOpened;
	OnRep_bLidOpened();
}


void ATItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// default rule that says to unconditionally replicate the value to all clients when changed
	DOREPLIFETIME(ATItemChest, bLidOpened);
}