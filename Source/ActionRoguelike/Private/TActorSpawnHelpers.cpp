#include "TActorSpawnHelpers.h"

#include "NavigationSystem.h"

void TActorSpawnHelpers::BreakVolumeIntoPointGrid(const FVector VolumeOrigin, const FVector VolumeExtent,
	const float CellCountX, const float CellCountY, TArray<FVector>& OutPoints)
{
	OutPoints.Reset(CellCountX * CellCountY);
	// Break bounding box up into grid, with each cell MinDistanceBetweenPickups in length
	const FVector GridTopLeft = FVector(VolumeOrigin.X - VolumeExtent.X,
	                                    VolumeOrigin.Y + VolumeExtent.Y,
	                                    VolumeOrigin.Z);
	const float CellLength = VolumeExtent.X * 2.f / CellCountX;

	OutPoints.Reserve(CellCountX * CellCountY);
	for (int32 x = 0; x <= CellCountX; x++)
	{
		for (int32 y = 0; y <= CellCountY; y++)
		{
			const FVector CellRelativeLocation = FVector(CellLength * x, CellLength * -y, 0.f);
			OutPoints.Add(FVector(GridTopLeft + CellRelativeLocation));	
		}
	}
}

void TActorSpawnHelpers::ProjectPointsToNav(UWorld* World, const TArray<FVector>& InPoints, TArray<FVector>& OutPoints)
{
	TArray<FNavigationProjectionWork> Workload;
	Workload.Reserve(InPoints.Num());
	for (auto& GridPoint : InPoints)
	{
		Workload.Add(FNavigationProjectionWork(GridPoint));
	}

	const FVector ProjectionExtent(0.f, 0.f, 1024.f);
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	const ANavigationData* NavData = NavSys->GetDefaultNavDataInstance(FNavigationSystem::ECreateIfEmpty::DontCreate);
	if (NavData)
	{
		NavData->BatchProjectPoints(Workload, ProjectionExtent);
	}
	
	OutPoints.Reserve(Workload.Num());
	for (auto& Work : Workload)
	{
		if (Work.bResult)
		{
			OutPoints.Add(Work.OutLocation.Location);
		}
	}
}
