#pragma once

class TActorSpawnHelpers
{
public:
	
	// Breaks a volume into an X by Y grid of square cells
	static void BreakVolumeIntoPointGrid(const FVector VolumeOrigin, const FVector VolumeExtent,
									      const float CellCountX, const float CellCountY, TArray<FVector>& OutPoints);

	// Project points to the world's default navigation mesh
	static void ProjectPointsToNav(UWorld* World, const TArray<FVector>& InPoints, TArray<FVector>& OutPoints);
	
};
