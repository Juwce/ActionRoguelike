// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPickupSpawnVolume.generated.h"


class UBoxComponent;
class ATPickupActor;

USTRUCT(BlueprintType)
struct FTPickupSpawnInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ATPickupActor> PickupClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 NumberToSpawn;

	TArray<FVector> SpawnPoints;
};

UCLASS(Blueprintable)
class ACTIONROGUELIKE_API ATPickupSpawnVolume : public AActor
{
	GENERATED_BODY()

public:
	ATPickupSpawnVolume();

	virtual void PostInitializeComponents() override;
	
	void DebugDrawSpawnPoints();

	void SpawnPickups();
	
protected:

	void ComputeCandidateSpawnPoints();

	void SelectSpawnPointsFromCandidates();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* VolumeBounds;
	
	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	float MinDistanceBetweenPickups;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	float ZOffsetFromGround;

	// Specify pickup classes to spawn, and how many of them
	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	TArray<FTPickupSpawnInfo> PickupActors;

	UPROPERTY()
	TArray<FVector> CandidateSpawnPoints;
	
	UPROPERTY(EditAnywhere, Category = "Editor Debugging")
	bool bDebugDrawSpawnPoints;
	
private:
	bool bSpawnPointsSelected;

#if WITH_EDITOR
public:
	virtual void PostEditMove(bool bFinished) override;
	void OnSelectionChanged(UObject* NewSelection);
	
private:
	bool SelectedInEditor;
#endif
	
};