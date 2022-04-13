// Fill out your copyright notice in the Description page of Project Settings.


#include "TGameModeBase.h"

#include "DrawDebugHelpers.h"
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

int32 ATGameModeBase::GetMaxBotCount()
{
	if (DifficultyCurve)
	{
		return static_cast<int32>(DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds));
	}
	else
	{
		return MaxBotCount;
	}
}

void ATGameModeBase::SpawnBotTimerElapsed()
{
	GetMaxBotCount();
	
	const int32 NumAliveBots = GetNumAliveBots();
	if (NumAliveBots >= GetMaxBotCount())
	{
		UE_LOG(LogTemp, Warning,
			TEXT("At maximum bot capacity (%i / %i). Skipping bot spawn."), NumAliveBots, GetMaxBotCount());
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance =
		UEnvQueryManager::RunEQSQuery(
			this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr
			);

	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ATGameModeBase::OnSpawnQueryComplete);
	}
}

int32 ATGameModeBase::GetNumAliveBots()
{
	int32 NumAliveBots = 0;
	for (TActorIterator<ATAICharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		const ATAICharacter* Bot = *Iter;
		if (UTAttributeComponent::IsActorAlive(Bot))
		{
			NumAliveBots++;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Found %i out of max %i alive bots."), NumAliveBots, GetMaxBotCount());

	return NumAliveBots;
}

void ATGameModeBase::CheatKillAllBots()
{
	for (TActorIterator<ATAICharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		ATAICharacter* Bot = *Iter;
		UTAttributeComponent* AttributeComp = UTAttributeComponent::GetAttributes(Bot);
		if (AttributeComp && AttributeComp->IsAlive())
		{
			// TODO: decide who kill credit goes to
			AttributeComp->Kill(this);
		}
	}
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
	
	GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
	DrawDebugSphere(GetWorld(), Locations[0], 50.f, 20, FColor::Blue, false, 60.f);
}
