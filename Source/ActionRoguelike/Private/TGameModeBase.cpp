// Fill out your copyright notice in the Description page of Project Settings.


#include "TGameModeBase.h"

#include "EngineUtils.h"
#include "TAttributeComponent.h"
#include "AI/TAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

ATGameModeBase::ATGameModeBase()
{
	SpawnTimerInterval = 2.f;
	MaxBotCount = 10.f;
}

void ATGameModeBase::StartPlay()
{
	Super::StartPlay();

	// Continuous timer to spawn in more bots.
	// Actual amount of bots and whether it's allowed to spawn determined by spawn log later in the chain...
	GetWorldTimerManager().SetTimer(
		TimerHandle_SpawnBots, this, &ATGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ATGameModeBase::SpawnBotTimerElapsed()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInstance =
		UEnvQueryManager::RunEQSQuery(
			this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr
			);

	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ATGameModeBase::OnSpawnQueryComplete);
	}
}

int32 ATGameModeBase::GetNumAliveBots() const
{
	int32 NumAliveBots = 0;
	for (TActorIterator<ATAICharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		ATAICharacter* Bot = *Iter;
		UTAttributeComponent* AttributeComp = Cast<UTAttributeComponent>(
			Bot->GetComponentByClass(UTAttributeComponent::StaticClass()));
		if (AttributeComp && AttributeComp->IsAlive())
		{
			NumAliveBots++;
		}
	}

	return NumAliveBots;
}

void ATGameModeBase::OnSpawnQueryComplete(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (!Locations.IsValidIndex(0))
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query could not find a suitable spawn location."));
		return;
	}

	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	
	const int32 NumAliveBots = GetNumAliveBots();
	if (NumAliveBots >= MaxBotCount)
	{
		return;
	}
	
	GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
}
