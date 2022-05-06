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
	
	UPROPERTY(EditAnywhere, Category = "Spawning | Pickups")
	float MinDistanceBetweenPickups;

	UPROPERTY(EditAnywhere, Category = "Spawning | Pickups")
	float ZOffsetFromGround;

	// Specify pickup classes to spawn, and how many of them
	UPROPERTY(EditDefaultsOnly, Category = "Spawning | Pickups")
	TArray<FTPickupSpawnInfo> PickupActors;

	UPROPERTY()
	TArray<FVector> CandidateSpawnPoints;

	// Draw all possible spawn candidate locations (projected onto the navmesh). Warning: this may be performance
	// intensive and lock up the editor if there are hundreds of potential spawn points
	UPROPERTY(EditAnywhere, Category = "Spawning | Editor Debugging")
	bool bDebugDrawSpawnCandidates;

	// Draw spawn points, randomly selected. Each Pickup Actor is assigned a random color.
	UPROPERTY(EditAnywhere, Category = "Spawning | Editor Debugging")
	bool bDebugDrawSampleSpawnPoints;
	
private:
	bool bSpawnPointsSelected;

#if WITH_EDITOR
public:
	virtual void PostEditMove(bool bFinished) override;
	void OnSelectionChanged(UObject* NewSelection);
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
private:
	bool SelectedInEditor;
#endif
	
};