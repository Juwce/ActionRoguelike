// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "GameFramework/GameModeBase.h"
#include "TGameModeBase.generated.h"

class ATPlayerState;
class ATPickupActor;
class UEnvQuery;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FTPickupSpawnInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ATPickupActor> PickupClass;

	UPROPERTY(EditAnywhere)
	int32 NumberToSpawn;
};

USTRUCT()
struct FTGridCell
{
	GENERATED_BODY()

	FTGridCell()
	{
		X = 0;
		Y = 0;
	}
	
	FTGridCell(const int32 XValue, const int32 YValue)
	{
		X = XValue;
		Y = YValue;
	}
	
	int32 X;
	int32 Y;
};

UCLASS()
class ACTIONROGUELIKE_API ATGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATGameModeBase();
	
	virtual void StartPlay() override;

	UFUNCTION(Exec)
	void CheatKillAllBots();
	
	void OnActorKilled(AActor* VictimActor, AActor* InstigatorActor);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game Credits")
	int32 BotKillCreditDelta;

	/*
	 * Pickup Spawning
	 */
protected:
	UFUNCTION(Exec)
	void SpawnPickups();

	// TODO: This is a bad implementation as it requires a specific instance of an actor that may not be
	// TODO: set on every level. Change to pick a spawn volume class, search the world for all volumes, and trigger
	// TODO: spawn logic accordingly. Spawn logic should probably be located in that class anyways and not in game mode.
	// Volume in which to spawn the pickup
	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	TSoftObjectPtr<AActor> PickupSpawnVolume;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	float MinDistanceBetweenPickups;

	// Specify pickup classes to spawn, and how many of them
	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	TArray<FTPickupSpawnInfo> PickupActors;
	
	/*
	 * Player Spawning
	 */
protected:
	UFUNCTION()
	void RespawnPlayer(AController* Controller);

	// Delay between requesting a player respawn and the respawn actually happening
	UPROPERTY(EditDefaultsOnly, Category = "Player Spawns")
	float PlayerRespawnDelay;

	/*
	 * Bot Spawning
	 */
public:
	UFUNCTION(BlueprintCallable)
	int32 GetMaxBotCount();

	int32 GetNumAliveBots();

protected:
	void TrySpawnBot();
	
	UFUNCTION()
	void OnSpawnBotQueryComplete(UEnvQueryInstanceBlueprintWrapper* EnvQueryInstanceBlueprintWrapper, EEnvQueryStatus::Type Arg);

	// EQS query that picks a location to spawn the bot
	UPROPERTY(EditDefaultsOnly, Category = "Bot Spawns")
	UEnvQuery* SpawnBotQuery;

	// Frequency at which to attempt to spawn a bot
	UPROPERTY(EditDefaultsOnly, Category = "Bot Spawns")
	float SpawnBotIntervalSeconds;

	// The bot actor class to spawn
	UPROPERTY(EditDefaultsOnly, Category = "Bot Spawns")
	TSubclassOf<AActor> SpawnedBotClass;

	// Max amount of bots allowed to spawn. Overridden by DifficultyCurve if that is set.
	UPROPERTY(EditDefaultsOnly, Category = "Bot Spawns")
	int32 MaxBotCount;

	// Curve determining max number of bots that can be spawned over the duration of the game. Overrides MaxBotCount if set.
	UPROPERTY(EditDefaultsOnly, Category = "Bot Spawns")
	UCurveFloat* DifficultyCurve;
	
private:
	FTimerHandle TimerHandle_SpawnBot;

};
