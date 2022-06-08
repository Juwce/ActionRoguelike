// Fill out your copyright notice in the Description page of Project Settings.


#include "TAttributeComponent.h"

#include "TGameModeBase.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(
	TEXT("ti.DamageMultiplier"), 1.f, TEXT("Global damage modifier for Attribute Component."), ECVF_Cheat);

// Sets default values for this component's properties
UTAttributeComponent::UTAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	HealthMax = 100.f;
	Health = HealthMax;

	RageMax = 50.f;
	Rage = 0.f;
	HealthToRageConversionRatio = 1.f;

	bCheat_TakeNoDamage = false;
	bCheat_TakeAlmostNoDamage = false;

	SetIsReplicatedByDefault((true));
}

void UTAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner()->HasAuthority())
	{
		OnHealthChanged.AddUniqueDynamic(this, &UTAttributeComponent::ConvertHealthChangeToRage);
	}
}

void UTAttributeComponent::ApplyRageChange(AActor* InstigatorActor, float Delta)
{
	if (GetOwner()->HasAuthority())
	{
		const float OldRage = Rage;
		const float NewRage = FMath::Clamp(Rage + Delta, 0.f, RageMax);

		const float AppliedDelta = NewRage - OldRage;
		SetRage(NewRage);
		MulticastRageChanged(InstigatorActor, NewRage, AppliedDelta);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] attempted to change [%s]'s rage but does not have authority!"),
			*GetNameSafe(InstigatorActor), *GetNameSafe(GetOwner()));
	}
}


void UTAttributeComponent::ConvertHealthChangeToRage(AActor* InstigatorActor, UTAttributeComponent* OwningComp,
	float NewHealth, float HealthDelta)
{
	// Rage only applies when damaged.
	if (HealthDelta > 0.f)
	{
		return;
	}

	const float RageDelta = -1.f * HealthDelta * HealthToRageConversionRatio;
	ApplyRageChange(InstigatorActor, RageDelta);
}

void UTAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor, this, NewRage, Delta);
}

void UTAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	// already dead
	if (Health == 0.f)
	{
		return;
	}
	
	if (bCheat_TakeNoDamage)
	{
		Delta = FMath::Max(Delta, 0.f);
	}
	else if (bCheat_TakeAlmostNoDamage)
	{
		Delta = FMath::Max(Delta, -1.f);
	}

	if (Delta < 0.f)
	{
		const float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}
	
	const float OldHealth = Health;
	const float NewHealth = FMath::Clamp(Health + Delta, 0.f, HealthMax);
	const float AppliedDelta = NewHealth - OldHealth;

	// only server should affect game state (player health, death, etc.)
	if (GetOwner()->HasAuthority())
	{
		SetHealth(NewHealth);
		
		// Died
		if (AppliedDelta < 0.f && Health == 0.f)
		{
			ATGameModeBase* GM = GetWorld()->GetAuthGameMode<ATGameModeBase>();
			if (GM)
			{
				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}

		// Broadcast last, listeners can respond to health changes with further health changes that we do not want
		// affecting the conditional checks above
		MulticastHealthChanged(InstigatorActor, NewHealth, AppliedDelta);
	}
}

void UTAttributeComponent::ApplyHealthChangeOverTime(AActor* InstigatorActor, const float Delta, const float Duration, const int32 Ticks)
{
	if (!ensure(Duration >= 0.f))
	{
		return;
	}
	if (!ensureMsgf(Ticks > 0, TEXT("Health Change over time must have at least 1 tick")))
	{
		return;
	}
	
	// TODO: Handle concurrent health change over time effects
	if (GetWorld()->GetTimerManager().IsTimerActive(HealthChangeTimerHandle))
	{
		return;
	}

	HealthChangeTicksLeft = Ticks;
	const float TickDelta = Delta / static_cast<float>(Ticks);
	const float TickDuration = Duration / static_cast<float>(Ticks);
	
	HealthChangeOverTime_Tick(InstigatorActor, TickDelta, TickDuration);
}

void UTAttributeComponent::StopHealthChangeOverTime()
{
	GetWorld()->GetTimerManager().ClearTimer(HealthChangeTimerHandle);
}

bool UTAttributeComponent::IsAlive() const
{
	return Health > 0.f;
}

void UTAttributeComponent::Kill(AActor* InstigatorActor)
{
	ApplyHealthChange(InstigatorActor, -GetHealth());
}

void UTAttributeComponent::MulticastHealthChanged_Implementation(AActor* Instigator, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(Instigator, this, NewHealth, Delta);
}

void UTAttributeComponent::HealthChangeOverTime_Tick(AActor* InstigatorActor, const float Delta, const float Duration)
{
	if (HealthChangeTicksLeft <= 0)
	{
		return;
	}
	
	ApplyHealthChange(InstigatorActor, Delta);
	HealthChangeTicksLeft--;
	
	FTimerDelegate TimerDel;
	TimerDel.BindLambda([=]() { HealthChangeOverTime_Tick(InstigatorActor, Delta, Duration); });
	GetWorld()->GetTimerManager().SetTimer(HealthChangeTimerHandle, TimerDel, Duration, false);
}

UTAttributeComponent* UTAttributeComponent::GetAttributes(const AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UTAttributeComponent>(FromActor->GetComponentByClass(UTAttributeComponent::StaticClass()));
	}
	
	return nullptr;
}

bool UTAttributeComponent::IsActorAlive(const AActor* Actor)
{
	const UTAttributeComponent* AttributeComponent = GetAttributes(Actor);
	if (AttributeComponent)
	{
		return AttributeComponent->IsAlive();
	}
	return false;
}

void UTAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTAttributeComponent, Health);
	DOREPLIFETIME(UTAttributeComponent, HealthMax);
	
	DOREPLIFETIME(UTAttributeComponent, Rage);
	DOREPLIFETIME(UTAttributeComponent, RageMax);
}