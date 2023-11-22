// Fill out your copyright notice in the Description page of Project Settings.

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

	~Tracer();

	// Pass the desired Object transform (center location in land and rotation) and extents to calculate Corners.
	void SetupTracer(UWorld* World, const FTransform WorldTransform, const float ExtentX, const float ExtentY);

	inline const float GetMaxHeightDistance();
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

	inline const bool GetTracedCorner(const int Index, FHitResult& OutHit);
};
