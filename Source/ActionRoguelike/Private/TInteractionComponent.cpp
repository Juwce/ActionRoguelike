// Fill out your copyright notice in the Description page of Project Settings.


#include "TInteractionComponent.h"

#include "DrawDebugHelpers.h"
#include "TGameplayInterface.h"


// Sets default values for this component's properties
UTInteractionComponent::UTInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MaxInteractDistance = 1000.f;

	// ..
}


// Called when the game starts
void UTInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UTInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic); // only interact with WorldDynamic

	AActor* MyOwner = GetOwner();

	FVector EyesLocation;
	FRotator EyesRotation;
	MyOwner->GetActorEyesViewPoint(EyesLocation, EyesRotation);

	const FVector TraceEnd = EyesLocation + (EyesRotation.Vector() * MaxInteractDistance);

	// Takes a sphere and "moves" it from start to end, stopping at its first blocking hit
	TArray<FHitResult> Hits;
	
	FCollisionShape Shape;
	float Radius = 30.f;
	Shape.SetSphere(Radius);
	
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyesLocation, TraceEnd, FQuat::Identity, ObjectQueryParams, Shape);

	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, FColor::Green, false, 2.f, 0, 0.5f);
		
		// Check the "U" TGameplayInterface when checking for Implementation
		if (HitActor && HitActor->Implements<UTGameplayInterface>())
		{
			APawn* MyPawn = Cast<APawn>(MyOwner);
			// Call interface functions with the "I" TGameplayInterface
			// BlueprintNative UFUNCTIONs are Static functions prefixed
			// with "Execute_" and called as follows:
			ITGameplayInterface::Execute_Interact(HitActor, MyPawn);
			break;
		}
	}

	const FColor DebugLineColor = bBlockingHit ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), EyesLocation, TraceEnd, DebugLineColor, false, 2.f, 0, 2.f);
}
