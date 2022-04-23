// Fill out your copyright notice in the Description page of Project Settings.


#include "TPickupSpawnVolume.h"

#include "DrawDebugHelpers.h"
#include "LevelEditor.h"
#include "TActorSpawnHelpers.h"
#include "TPickupActor.h"
#include "Components/BoxComponent.h"
#include "Engine/Selection.h"

ATPickupSpawnVolume::ATPickupSpawnVolume()
{
	VolumeBounds = CreateDefaultSubobject<UBoxComponent>("VolumeBounds");
	RootComponent = VolumeBounds;

	MinDistanceBetweenPickups = 100.f;
	ZOffsetFromGround = 50.f;

	bDebugDrawSpawnPoints = false;
	
	#if WITH_EDITOR
	// TODO: this currently works in the editor, but since these bindings are being done in the constructor, I need to
	// re-create the blueprint asset from this class to make sure OnObjectSelected() is not still being called
	// and causing the correct functionality that I am seeing in the editor
	// Unreal's EQSTestingPawn.cpp binds both SelectionChangedEvent AND OnObjectSelected here, but I am able to get
	// the correct functionality with just SelectionChangedEvent...so it makes me wonder if I am missing something
	USelection::SelectionChangedEvent.AddUObject(this, &ATPickupSpawnVolume::OnSelectionChanged);
	#endif
}

void ATPickupSpawnVolume::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}


void ATPickupSpawnVolume::DebugDrawSpawnPoints()
{
	FlushPersistentDebugLines(GetWorld());

	if (bDebugDrawSpawnPoints)
	{
		ComputeCandidateSpawnPoints();
		SelectSpawnPointsFromCandidates();
		for (const auto& Point : CandidateSpawnPoints)
		{
			DrawDebugSphere(GetWorld(), Point, 24.f, 12.f, FColor::Green, true);
		}
				
		for (const auto& PickupSpawnInfo : PickupActors)
		{
			for (auto& Point : PickupSpawnInfo.SpawnPoints)
			{
				DrawDebugSphere(GetWorld(), Point, 32.f, 12.f, FColor::Blue, true);
			}
		}
	}
}

void ATPickupSpawnVolume::SpawnPickups()
{
	ComputeCandidateSpawnPoints();
	SelectSpawnPointsFromCandidates();

	for (const FTPickupSpawnInfo& PickupSpawnInfo : PickupActors)
	{
		for (const FVector& Point : PickupSpawnInfo.SpawnPoints)
		{
			GetWorld()->SpawnActor<AActor>(
				PickupSpawnInfo.PickupClass,
				Point,
				FRotator(0.f, FMath::RandRange(0.f, 360.f), 0.f));
		}
	}
}

void ATPickupSpawnVolume::ComputeCandidateSpawnPoints()
{
	CandidateSpawnPoints.Empty();
	
	FVector VolumeOrigin;
	FVector VolumeExtent;
	this->GetActorBounds(false, VolumeOrigin, VolumeExtent);

	TArray<FVector> GridPoints;
	const int32 CellCountX = FMath::Floor(VolumeExtent.X * 2 / MinDistanceBetweenPickups);
	const int32 CellCountY = FMath::Floor(VolumeExtent.Y * 2 / MinDistanceBetweenPickups);
	TActorSpawnHelpers::BreakVolumeIntoPointGrid(VolumeOrigin, VolumeExtent, CellCountX, CellCountY, GridPoints);

	TActorSpawnHelpers::ProjectPointsToNav(GetWorld(), GridPoints, CandidateSpawnPoints);
	
	UE_LOG(LogTemp, Verbose, TEXT("GameMode SpawnPickups: %d / %d spawn points found."), CandidateSpawnPoints.Num(), GridPoints.Num());

	bSpawnPointsSelected = false;
}

void ATPickupSpawnVolume::SelectSpawnPointsFromCandidates()
{
	TArray<FVector> CandidateSpawnPointsCopy(CandidateSpawnPoints);
 	for (FTPickupSpawnInfo& PickupSpawnInfo : PickupActors)
	{
 		PickupSpawnInfo.SpawnPoints.Empty();
		for (int i = 0; i < PickupSpawnInfo.NumberToSpawn; i++)
		{
			if (CandidateSpawnPointsCopy.Num() == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("Volume '%s' ran out of valid spawn points for pickups. Try "
											  "show SpawnCandidates to preview pickup spawn locations."),
											  *GetNameSafe(this));
				break;
			}
			
			const int32 RandIndex = FMath::RandRange(0, CandidateSpawnPointsCopy.Num() - 1);
			const FVector SpawnPoint = CandidateSpawnPointsCopy[RandIndex] + FVector(0.f, 0.f, ZOffsetFromGround);
			PickupSpawnInfo.SpawnPoints.Add(SpawnPoint);
			CandidateSpawnPointsCopy.RemoveAt(RandIndex);
		}
	}

	bSpawnPointsSelected = true;
}


#if WITH_EDITOR
void ATPickupSpawnVolume::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	if (bFinished /* && SelectedInEditor */)
	{
		UE_LOG(LogTemp, Warning, TEXT("DEBUG MOVED %s"), *GetNameSafe(this));
		DebugDrawSpawnPoints();
	}
}

void ATPickupSpawnVolume::OnSelectionChanged(UObject* NewSelection)
{
	UE_LOG(LogTemp, Warning, TEXT("Selection Changed!"));
	TArray<ATPickupSpawnVolume*> SelectedVolumes;
	ATPickupSpawnVolume* SelectedVolume = Cast<ATPickupSpawnVolume>(NewSelection);
	if (SelectedVolume)
	{
		SelectedVolumes.Add(Cast<ATPickupSpawnVolume>(NewSelection));
	}
	else 
	{
		USelection* Selection = Cast<USelection>(NewSelection);
		if (Selection != nullptr)
		{
			Selection->GetSelectedObjects<ATPickupSpawnVolume>(SelectedVolumes);
		}
	}

	for (ATPickupSpawnVolume* SpawnVolume : SelectedVolumes)
	{
		if (SpawnVolume == this && !SelectedInEditor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Selected for the first time! %s"), *GetNameSafe(SpawnVolume));
			DebugDrawSpawnPoints();
			SelectedInEditor = true;
		}
	}

	if (SelectedInEditor && !IsSelected())
	{
		UE_LOG(LogTemp, Warning, TEXT("Deselected! %s"), *GetNameSafe(this));
		FlushPersistentDebugLines(GetWorld());
		SelectedInEditor = false;
	}
}
#endif
