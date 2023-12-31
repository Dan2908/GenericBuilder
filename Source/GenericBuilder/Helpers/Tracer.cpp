// Author: Danilo Brandolin

#include "Tracer.h"

// Constructor
Tracer::Tracer()
	: CurrentWorld(nullptr)
	, CornerA()
	, CornerB()
	, CornerC()
	, CornerD()
{}
// ---------------------------------------------------------------

// Destructor
Tracer::~Tracer()
{}
// ---------------------------------------------------------------

// Initializer constructor, initializes all parameters calling SetupTracer()
Tracer::Tracer(UWorld* World, const FTransform WorldTransform, const float ExtentX, const float ExtentY)
	: Tracer()
{
	SetupTracer(World, WorldTransform, ExtentX, ExtentY);

}
// ---------------------------------------------------------------

// Pass the building transform (center location in land and rotation) and extents to calculate Corners.
void Tracer::SetupTracer(UWorld* World, const FTransform WorldTransform, const float ExtentX, const float ExtentY)
{
	check(World);

	CurrentWorld = World;

	const FVector Location = WorldTransform.GetLocation();
	const FVector Forward = WorldTransform.GetRotation().GetForwardVector() * ExtentX;
	const FVector Right = WorldTransform.GetRotation().GetRightVector() * ExtentY;

	CornerA = Location + (Forward + Right);
	CornerB = Location + (Forward - Right);
	CornerC = Location + (-Forward + Right);
	CornerD = Location + (-Forward - Right);

	// Trace corners to land
	for (int i = 0; i < 4; ++i)
	{
		FHitResult Hit;
		GetTracedCorner(i, Hit);

		Corners[i] = Hit.Location;

		DrawDebugSolidBox(World, Hit.Location, { 25, 25, 25 }, FColor::Blue);
	}
}
// ---------------------------------------------------------------

// Get the distance between the highest and the lowest corner projected to the ground.
inline const float Tracer::GetCornersDiff()
{
	return GetHighestCorner() - GetLowestCorner();
}
// ---------------------------------------------------------------

// Get the highest corner location.
inline const float Tracer::GetHighestCorner()
{
	return std::fmax(Corners[0].Z,
			  std::fmax(Corners[1].Z, 
				 std::fmax(Corners[2].Z, Corners[3].Z)));
}
// ---------------------------------------------------------------

// Get the lowest corner location.
inline const float Tracer::GetLowestCorner()
{
	return std::fmin(Corners[0].Z, 
			  std::fmin(Corners[1].Z, 
				 std::fmin(Corners[2].Z, Corners[3].Z)));
}
// ---------------------------------------------------------------

// Trace Line to ground given a point in the world. It takes the point and projects it
// through the Z axis and hits "Landscape" channel.
inline FVector Tracer::TraceGround(UObject* WorldObject, const FVector Location)
{
	const FVector UpDistance = FVector::UpVector * sTracingHalfDistance;

	FHitResult Hit;
	const bool Success =
		WorldObject->GetWorld()->LineTraceSingleByChannel(
			Hit,
			Location + UpDistance,
			Location - UpDistance,
			ECollisionChannel::ECC_GameTraceChannel1
		);

	return Success ? Hit.Location : Location;
}
// ---------------------------------------------------------------


// Get the highest location projected to the ground.
inline const float Tracer::GetHighestPoint(UObject* WorldObject, const FBox& PlacementBox, const FVector Location)
{
	const FVector UpDistance = FVector::UpVector * sTracingHalfDistance;

	FHitResult Hit;
	WorldObject->GetWorld()->SweepSingleByChannel
	(
		Hit,
		Location + UpDistance,
		Location - UpDistance,
		FRotator::ZeroRotator.Quaternion(),	// no rotation
		ECollisionChannel::ECC_GameTraceChannel1,	// ground channel
		FCollisionShape::MakeBox(PlacementBox.GetExtent()) // Make a box from PlacementBox (divide by 2 tp get half extents)
	);

	return Hit.Location.Z;
}
// ---------------------------------------------------------------

// Fixes given Location rounding it to the given StepSize.
inline const void Tracer::FixLocationToGrid(FVector& Location, const float StepSize)
{
	Location.X = ceil(Location.X / StepSize) * StepSize;
	Location.Y = ceil(Location.Y / StepSize) * StepSize;
}
// ---------------------------------------------------------------

// Trace a line from each corner, the result is stored over corners array.
inline const bool Tracer::GetTracedCorner(const int Index, FHitResult& OutHit)
{
	check(Index > -1 && Index < 4);

	return CurrentWorld->LineTraceSingleByChannel(
		OutHit, 
		Corners[Index] + FVector::UpVector * sTracingHalfDistance, // Start
		Corners[Index] + FVector::DownVector * sTracingHalfDistance, // End
		ECollisionChannel::ECC_GameTraceChannel1);
}
// ---------------------------------------------------------------
