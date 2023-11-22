// Fill out your copyright notice in the Description page of Project Settings.

#include "Tracer.h"


// ---------------------------------------------------------------
// -------------------------- PUBLIC --------------------------
// ---------------------------------------------------------------

Tracer::Tracer()
	: CornerA()
	, CornerB()
	, CornerC()
	, CornerD()
{}
// ---------------------------------------------------------------

// Initializer constructor, initializes all parameters calling SetupTracer()
Tracer::Tracer(UWorld* World, const FTransform WorldTransform, const float ExtentX, const float ExtentY)
	: Tracer()
{
	SetupTracer(World, WorldTransform, ExtentX, ExtentY);
}
// ---------------------------------------------------------------

Tracer::~Tracer()
{}
// ---------------------------------------------------------------

// Pass the building transform (center location in land and rotation) and extents to calculate Corners.
void Tracer::SetupTracer(UWorld* World, const FTransform WorldTransform, const float ExtentX, const float ExtentY)
{
	check(World);

	CurrentWorld = World;

	const FVector Location = WorldTransform.GetLocation();
	const FVector Forward = WorldTransform.GetRotation().GetForwardVector();
	const FVector Right = WorldTransform.GetRotation().GetRightVector();

	CornerA = Location * (Forward + Right);
	CornerB = Location * (Forward - Right);
	CornerC = Location * (-Forward + Right);
	CornerD = Location * (-Forward - Right);

	// Trace corners to land
	for (int i = 0; i < 4; ++i)
	{
		FHitResult Hit;
		GetTracedCorner(i, Hit);

		Corners[i] = Hit.Location;
	}
}


inline const float Tracer::GetMaxHeightDistance()
{
	// Trace corners to land
	for (int i = 0; i < 4; ++i)
	{
		FHitResult Hit;
		GetTracedCorner(i, Hit);

		Corners[i] = Hit.Location;
	}

	return /* Max Z - Min Z*/
		std::fmax(Corners[0].Z, std::fmax(Corners[1].Z, std::fmax(Corners[2].Z, Corners[3].Z)))
		- std::fmin(Corners[0].Z, std::fmin(Corners[1].Z, std::fmin(Corners[2].Z, Corners[3].Z)));
}
// ---------------------------------------------------------------

// ---------------------------------------------------------------
// -------------------------- PRIVATE --------------------------
// ---------------------------------------------------------------

inline const bool Tracer::GetTracedCorner(const int Index, FHitResult& OutHit)
{
	check(Index > -1 && Index < 4);

	return CurrentWorld->LineTraceSingleByChannel(
		OutHit, 
		Corners[Index] + FVector::UpVector * sTracingHalfDistance, // Start
		Corners[Index] + FVector::DownVector * sTracingHalfDistance, // End
		ECollisionChannel::ECC_GameTraceChannel1);
}
