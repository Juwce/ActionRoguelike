// Fill out your copyright notice in the Description page of Project Settings.


#include "TPickupSpawnVolume.h"

#include "DrawDebugHelpers.h"
#include "TActorSpawnHelpers.h"
#include "TPickupActor.h"
#include "Components/BoxComponent.h"
#include "Engine/Selection.h"

#if WITH_EDITOR
#include "LevelEditor.h"
#endif

ATPickupSpawnVolume::ATPickupSpawnVolume()
{
	VolumeBounds = CreateDefaultSubobject<UBoxComponent>("VolumeBounds");
	VolumeBounds->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = VolumeBounds;

	MinDistanceBetweenPickups = 100.f;
	ZOffsetFromGround = 50.f;

	bDebugDrawSpawnCandidates = false;
	bDebugDrawSampleSpawnPoints = false;
	
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

	if (bDebugDrawSpawnCandidates || bDebugDrawSampleSpawnPoints)
	{
		ComputeCandidateSpawnPoints();
		SelectSpawnPointsFromCandidates();
		
		if (bDebugDrawSpawnCandidates)
		{
			for (const auto& Point : CandidateSpawnPoints)
			{
				DrawDebugSphere(GetWorld(), Point, 24.f, 6.f, FColor::Green, true);
			}
		}

		if (bDebugDrawSampleSpawnPoints)
		{
			for (const auto& PickupSpawnInfo : PickupActors)
			{
				FColor PickupColor = FColor::MakeRandomColor();
				for (auto& Point : PickupSpawnInfo.SpawnPoints)
				{
					DrawDebugSphere(GetWorld(), Point, 32.f, 12.f, PickupColor, true);
				}
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
		DebugDrawSpawnPoints();
	}
}

void ATPickupSpawnVolume::OnSelectionChanged(UObject* NewSelection)
{
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
			DebugDrawSpawnPoints();
			SelectedInEditor = true;
		}
	}

	if (SelectedInEditor && !IsSelected())
	{
		FlushPersistentDebugLines(GetWorld());
		SelectedInEditor = false;
	}
}

void ATPickupSpawnVolume::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property == nullptr)
	{
		return;
	}
	
	const FName PropName = PropertyChangedEvent.GetPropertyName();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *PropName.ToString());

	if (PropName == GET_MEMBER_NAME_CHECKED(ATPickupSpawnVolume, bDebugDrawSpawnCandidates) ||
		PropName == GET_MEMBER_NAME_CHECKED(ATPickupSpawnVolume, bDebugDrawSampleSpawnPoints))
	{
		if (bDebugDrawSpawnCandidates || bDebugDrawSampleSpawnPoints)
		{
			DebugDrawSpawnPoints();
		}
		else
		{
			FlushPersistentDebugLines(GetWorld());
		}
	}
}
#endif
