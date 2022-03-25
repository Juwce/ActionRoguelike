// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "GameFramework/GameModeBase.h"
#include "TGameModeBase.generated.h"

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

protected:
	void SpawnBotTimerElapsed();

	int32 GetNumAliveBots() const;

	UFUNCTION()
	void OnSpawnQueryComplete(UEnvQueryInstanceBlueprintWrapper* EnvQueryInstanceBlueprintWrapper, EEnvQueryStatus::Type Arg);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	// Max amount of bots allowed to spawn. Overridden by DifficultyCurve if that is set.
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	int32 MaxBotCount;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;
	
	FTimerHandle TimerHandle_SpawnBots;

};
