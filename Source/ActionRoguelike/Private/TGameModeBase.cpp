// Fill out your copyright notice in the Description page of Project Settings.


#include "TGameModeBase.h"

#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "NavigationSystem.h"
#include "TActorSpawnHelpers.h"
#include "TAttributeComponent.h"
#include "TCharacter.h"
#include "TPickupActor.h"
#include "TPlayerController.h"
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
	MinDistanceBetweenPickups = 10.f;
}

void ATGameModeBase::StartPlay()
{
	Super::StartPlay();

	// Continuous timer to spawn in more bots.
	// Actual amount of bots and whether it's allowed to spawn determined by spawn log later in the chain...
	GetWorldTimerManager().SetTimer(
		TimerHandle_SpawnBot, this, &ATGameModeBase::TrySpawnBot, SpawnBotIntervalSeconds, true);

	SpawnPickups();
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
	// If the player died, respawn them on a delay
	ATCharacter* Player = Cast<ATCharacter>(VictimActor);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayer", Player->GetController());
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, PlayerRespawnDelay, false);
	}

	// If a bot died and a player controlled pawn killed it, assign credit to that player
	const bool bVictimIsAICharacter = Cast<ATAICharacter>(VictimActor) != nullptr;
	if (bVictimIsAICharacter)
	{
		const APawn* InstigatorPawn = Cast<APawn>(InstigatorActor);
		if (InstigatorPawn)
		{
			ATPlayerController* InstigatorPlayer = Cast<ATPlayerController>(InstigatorPawn->GetController());
			if (InstigatorPlayer)
			{
				InstigatorPlayer->ApplyCreditChange(BotKillCreditDelta);
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(InstigatorActor));
}


void ATGameModeBase::SpawnPickups()
{
	FVector VolumeOrigin;
	FVector VolumeExtent;
	PickupSpawnVolume->GetActorBounds(false, VolumeOrigin, VolumeExtent);

	TArray<FVector> GridPoints;
	const int32 CellCountX = FMath::Floor(VolumeExtent.X * 2 / MinDistanceBetweenPickups);
	const int32 CellCountY = FMath::Floor(VolumeExtent.Y * 2 / MinDistanceBetweenPickups);
	TActorSpawnHelpers::BreakVolumeIntoPointGrid(VolumeOrigin, VolumeExtent, CellCountX, CellCountY, GridPoints);

	TArray<FVector> SpawnPoints;
	TActorSpawnHelpers::ProjectPointsToNav(GetWorld(), GridPoints, SpawnPoints);
	
	UE_LOG(LogTemp, Warning, TEXT("GameMode SpawnPickups: %d / %d spawn points found."), SpawnPoints.Num(), GridPoints.Num());

	for (const FTPickupSpawnInfo& PickupSpawnInfo : PickupActors)
	{
		for (int i = 0; i < PickupSpawnInfo.NumberToSpawn; i++)
		{
			if (SpawnPoints.Num() == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("GameMode: Ran out of spawn points for pickups."))
				break;
			}
			
			const int32 RandIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
			const FVector SpawnPoint = SpawnPoints[RandIndex] + FVector(0.f, 0.f, ZOffsetFromGround);
			SpawnPoints.RemoveAt(RandIndex);

			DrawDebugSphere(GetWorld(), SpawnPoint, 20.f, 12.f, FColor::Green, false, 5.f);
			GetWorld()->SpawnActor<AActor>(
				PickupSpawnInfo.PickupClass,
				SpawnPoint,
				FRotator(0.f, FMath::RandRange(0.f, 360.f), 0.f));
		}
	}
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
