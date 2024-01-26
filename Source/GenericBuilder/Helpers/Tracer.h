// Author: Danilo Brandolin

#pragma once

#include "CoreMinimal.h"

// Distance from the center trace line to the start and end trace points
static const float sTracingHalfDistance = 600;

/**
 * 
 */
class GENERICBUILDER_API Tracer
{

public:
	Tracer();

	Tracer(UWorld* World, const FTransform WorldTransform, const float ExtentX, const float ExtentY);

	Tracer(UWorld* World, const FTransform WorldTransform, const FVector2D Extents);

	~Tracer();

	// Pass the desired Object transform (center location in land and rotation) and extents to calculate Corners.
	void SetupTracer(UWorld* World, const FTransform WorldTransform, const float ExtentX, const float ExtentY);

	// Get the distance between the highest and the lowest corner projected to the ground.
	inline const float GetCornersDiff();

	// Get the highest corner location.
	inline const float GetHighestCorner();

	// Get the lowest corner location.
	inline const float GetLowestCorner();

	// Trace Line to ground given a point in the world. It takes the point and projects it
	// through the Z axis and hits "Landscape" channel.
	static inline FVector TraceGround(UObject* WorldObject, const FVector Location);

	// Get the highest location projected to the ground.
	static inline const float GetHighestPoint(UObject* WorldObject, const FBox& PlacementBox, const FVector Location);

	// Fixes given Location rounding it to the given StepSize.
	static inline const void FixLocationToGrid(FVector& Location, const float StepSize);

private:

	UWorld* CurrentWorld;

	union
	{
		struct
		{
			FVector CornerA;
			FVector CornerB;
			FVector CornerC;
			FVector CornerD;
		};

		FVector Corners[4];
	};

	// Trace a line from each corner, the result is stored over corners array.
	inline const bool GetTracedCorner(const int Index, FHitResult& OutHit);
};
