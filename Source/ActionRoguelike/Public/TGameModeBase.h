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

	// Amount of credits to add/subtract when a coin is picked up
	UPROPERTY(EditDefaultsOnly, Category = "Game Credits")
	int32 CoinPickupCreditDelta;

	// Number of credits to add/subtract when a health pickup is picked up
	UPROPERTY(EditDefaultsOnly, Category = "Game Credits")
	int32 HealthPickupCreditDelta;

	UPROPERTY(EditDefaultsOnly, Category = "Game Credits")
	int32 BotKillCreditDelta;
	
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
