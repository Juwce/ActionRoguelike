// Fill out your copyright notice in the Description page of Project Settings.


#include "TGameModeBase.h"

#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "TAttributeComponent.h"
#include "TCharacter.h"
#include "TPlayerState.h"
#include "AI/TAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(
	TEXT("ti.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

ATGameModeBase::ATGameModeBase()
{
	SpawnBotIntervalSeconds = 2.f;
	MaxBotCount = 10.f;
	PlayerRespawnDelay = 2.f;

	PlayerStateClass = ATPlayerState::StaticClass();
}

void ATGameModeBase::StartPlay()
{
	Super::StartPlay();

	// Continuous timer to spawn in more bots.
	// Actual amount of bots and whether it's allowed to spawn determined by spawn log later in the chain...
	GetWorldTimerManager().SetTimer(
		TimerHandle_SpawnBot, this, &ATGameModeBase::TrySpawnBot, SpawnBotIntervalSeconds, true);
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

void ATGameModeBase::OnActorKilled(AActor* VictimActor, AActor* InstigatorActor)
{
	ATCharacter* Player = Cast<ATCharacter>(VictimActor);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayer", Player->GetController());
		
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, PlayerRespawnDelay, false);
	}

	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(InstigatorActor));
}

/*
 * Player Spawning
 */
void ATGameModeBase::RespawnPlayer(AController* Controller)
{
	if (Controller)
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}

/*
 * Bot Spawning
 */
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

void ATGameModeBase::TrySpawnBot()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via cvar \"CVarSpawnBots\""));
		return;
	}
	
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
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ATGameModeBase::OnSpawnBotQueryComplete);
	}
}

void ATGameModeBase::OnSpawnBotQueryComplete(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
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
	
	GetWorld()->SpawnActor<AActor>(SpawnedBotClass, Locations[0], FRotator::ZeroRotator);
	DrawDebugSphere(GetWorld(), Locations[0], 50.f, 20, FColor::Blue, false, 60.f);
}
