// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "GameFramework/GameModeBase.h"
#include "TGameModeBase.generated.h"

class UTMonsterData;
class UTSaveGame;
class ATPlayerState;
class ATPickupActor;
class UEnvQuery;
class UDataTable;
/**
 * 
 */

/* DataTable Row for spawning monsters in game mode  */
USTRUCT(BlueprintType)
struct FMonsterInfoRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	FMonsterInfoRow()
	{
		Weight = 1.0f;
		SpawnCost = 5.0f;
		KillReward = 20.0f;
	}

	// Final implementation, using asset manager
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId MonsterId;

	// Second iteration, using a data table of data assets
	// UPROPERTY(EditAnywhere, BlueprintReadOnly)
	// UTMonsterData* MonsterData;

	// First iteration, just specifying the class to spawn
	// UPROPERTY(EditAnywhere, BlueprintReadOnly)
	// TSubclassOf<AActor> MonsterClass;

	/* Relative chance to pick this monster */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;

	/* Points required by gamemode to spawn this unit. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost;

	/* Amount of credits awarded to killer of this unit.  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KillReward;

};


UCLASS()
class ACTIONROGUELIKE_API ATGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATGameModeBase();
	
	virtual void StartPlay() override;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	UFUNCTION(Exec)
	void CheatKillAllBots();
	
	void OnActorKilled(AActor* VictimActor, AActor* InstigatorActor);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game Credits")
	int32 BotKillCreditDelta;

	UFUNCTION(Exec)
	void SpawnPickups();
	
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* MonsterTable;

protected:
	void TrySpawnBot();

	UFUNCTION()
	void OnSpawnBotQueryComplete(UEnvQueryInstanceBlueprintWrapper* EnvQueryInstanceBlueprintWrapper, EEnvQueryStatus::Type Arg);

	void OnMonsterLoaded(FPrimaryAssetId MonsterAssetId, FVector SpawnLocation);
	
	void DoSpawnBot(const UTMonsterData* MonsterData, const FVector SpawnLocation);

	// EQS query that picks a location to spawn the bot
	UPROPERTY(EditDefaultsOnly, Category = "Bot Spawns")
	UEnvQuery* SpawnBotQuery;

	// Frequency at which to attempt to spawn a bot
	UPROPERTY(EditDefaultsOnly, Category = "Bot Spawns")
	float SpawnBotIntervalSeconds;

	// Max amount of bots allowed to spawn. Overridden by DifficultyCurve if that is set.
	UPROPERTY(EditDefaultsOnly, Category = "Bot Spawns")
	int32 MaxBotCount;

	// Curve determining max number of bots that can be spawned over the duration of the game. Overrides MaxBotCount if set.
	UPROPERTY(EditDefaultsOnly, Category = "Bot Spawns")
	UCurveFloat* DifficultyCurve;
	
private:
	FTimerHandle TimerHandle_SpawnBot;


	/*
	 * Saving and Loading
	 */
public:
	FString SaveSlotName;

	UPROPERTY()
	UTSaveGame* CurrentSaveGame;
	
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();
	
	void SavePlayerStates(UTSaveGame* SaveGame) const;
	void SaveGameplayActors(UTSaveGame* SaveGame) const;

	UFUNCTION()
	void LoadSaveGame();
};
