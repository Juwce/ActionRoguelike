// Fill out your copyright notice in the Description page of Project Settings.


#include "TInteractionComponent.h"

#include "DrawDebugHelpers.h"
#include "TGameplayInterface.h"
#include "TWorldUserWidget.h"
#include "Camera/CameraComponent.h"

static TAutoConsoleVariable<bool> CVarDebugInteractionComponent(
	TEXT("ti.DebugInteractionComponent"),
	false,
	TEXT("Draws interaction component debug helpers to the screen."),
	ECVF_Cheat);

// Sets default values for this component's properties
UTInteractionComponent::UTInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MaxInteractDistance = 1000.f;
}


void UTInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn && MyPawn->IsLocallyControlled())
	{
		UpdateInteractionWidgetAttachment(SelectInteractionTarget());
	}
}


void UTInteractionComponent::PrimaryInteract()
{
	const APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn && MyPawn->IsLocallyControlled())
	{
		ServerInteract(SelectInteractionTarget());
	}
}


void UTInteractionComponent::ServerInteract_Implementation(AActor* InteractionTarget)
{
	if (!InteractionTarget)
	{
		return;
	}
	
	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (!ensure(MyPawn))
	{
		return;
	}
	
	// Call interface functions with the "I" TGameplayInterface
	// BlueprintNative UFUNCTIONs are Static functions prefixed
	// with "Execute_" and called as follows:
	ITGameplayInterface::Execute_Interact(InteractionTarget, MyPawn);
}


AActor* UTInteractionComponent::SelectInteractionTarget()
{
	TArray<FHitResult> Hits;
	ComputeInteractCandidates(Hits);
	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor && HitActor->Implements<UTGameplayInterface>())
		{
			return HitActor;
		}
	}
	return nullptr;
}


bool UTInteractionComponent::ComputeInteractCandidates(TArray<FHitResult>& Hits)
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic); // only interact with WorldDynamic

	// Trace Start/End
	const AActor* MyOwner = GetOwner();
	FVector TraceStart;
	FRotator TraceRotation;
	const UCameraComponent* OwnersCamera = Cast<UCameraComponent>(
		MyOwner->GetComponentByClass(UCameraComponent::StaticClass()));
	if (OwnersCamera)
	{
		TraceStart = OwnersCamera->GetComponentLocation();
		TraceRotation = OwnersCamera->GetComponentRotation();
	}
	else
	{
		MyOwner->GetActorEyesViewPoint(TraceStart, TraceRotation);
	}
	const FVector TraceEnd = TraceStart + (TraceRotation.Vector() * MaxInteractDistance);
	
	const float Radius = 30.f;
	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	const bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, TraceStart, TraceEnd, FQuat::Identity, ObjectQueryParams, Shape);

	// Debug
	bool bDrawDebug = CVarDebugInteractionComponent.GetValueOnGameThread();
	if (bDrawDebug)
	{
		const FColor DebugLineColor = bBlockingHit ? FColor::Green : FColor::Red;
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, DebugLineColor, false, 2.f, 0, 2.f);
		for (FHitResult Hit : Hits)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, FColor::Green, false, 2.f, 0, 0.5f);
		}
	}

	return bBlockingHit;
}


void UTInteractionComponent::UpdateInteractionWidgetAttachment(AActor* ActorToAttachTo)
{
	if (ActorToAttachTo)
	{
		if (!InteractionWidgetInstance && ensure(InteractionWidgetClass))
		{
			InteractionWidgetInstance = CreateWidget<UTWorldUserWidget>(GetWorld(), InteractionWidgetClass);
		}
		
		if (!InteractionWidgetInstance)
		{
			// Widget creation failed
			return;
		}

		InteractionWidgetInstance->SetAttachedActor(ActorToAttachTo);
		
		if (!InteractionWidgetInstance->IsInViewport())
		{
			InteractionWidgetInstance->AddToViewport();
		}
	}
	else if (InteractionWidgetInstance && InteractionWidgetInstance->IsInViewport())
	{
		InteractionWidgetInstance->RemoveFromViewport();
	}
}
