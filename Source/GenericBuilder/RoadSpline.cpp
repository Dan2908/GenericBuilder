// Author: Danilo Brandolin


#include "RoadSpline.h"
#include "Helpers/Tracer.h"
#include "GenericBuilderGameModeBase.h"
#include "Game/ResourceCollection.h"

#include "Components/SplineComponent.h"

// Sets default values
ARoadSpline::ARoadSpline()
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>(FName("Spline"));

	RootComponent = SplineComponent;

	ProjectSplinePointsToGround();

}
// ---------------------------------------------------------------

// Called when the game starts or when spawned
void ARoadSpline::BeginPlay()
{
	Super::BeginPlay();
	
}
// ---------------------------------------------------------------


// Extend road one point forward 
void ARoadSpline::ExtendRoad()
{
	int PointIndex = SplineComponent->GetNumberOfSplinePoints() - 1;
	FVector NewPointLocation;
	FVector Tangent;

	SplineComponent->GetLocalLocationAndTangentAtSplinePoint(PointIndex, NewPointLocation, Tangent);

	NewPointLocation += GetActorForwardVector() * FVector(0, 0, TileExtent);
	
	SplineComponent->AddSplineLocalPoint(NewPointLocation);

	ProjectSplinePointsToGround();
}
// ---------------------------------------------------------------

// Decrease road one point backward 
void ARoadSpline::DecreaseRoad()
{
	const int PointIndex = SplineComponent->GetNumberOfSplinePoints() - 1;

	SplineComponent->RemoveSplinePoint(PointIndex);
}
// ---------------------------------------------------------------

// Fixes the current starting point, if possible, to the landscape point
void ARoadSpline::FixStartPoint()
{
	bIsStartFixed = true;
}
// ---------------------------------------------------------------


/*
const bool ARoadSpline::IsObstructed()
{
	return false;
}

const bool ARoadSpline::IsLandRight()
{
	return false;
}

inline bool ARoadSpline::Confirm()
{
	return false;
}
// ---------------------------------------------------------------

// Get Construction Cost for this building from the current available buildings from the current collection.
const FResourceVault& ARoadSpline::GetConstructionCost()
{
	return FResourceVault();
}
// ---------------------------------------------------------------

// Gets the calculated extents for this buildable
inline FVector2D ARoadSpline::GetExtents() const
{
	return FVector2D();
}
// ---------------------------------------------------------------

// Adjust spline point locations to the ground
void ARoadSpline::ProjectSplinePointsToGround()
{
	const int SplinePointsCount = SplineComponent->GetNumberOfSplinePoints();
	for (int i = 0; i < SplinePointsCount; ++i)
	{
		const FVector TracedLocation = Tracer::TraceGround(this, SplineComponent->GetWorldLocationAtSplinePoint(i));
		
		SplineComponent->SetWorldLocationAtSplinePoint(i, TracedLocation);

	}
}
// ---------------------------------------------------------------


const bool ARoadSpline::MoveBuildable(const FVector MouseLandLocation)
{
	FVector NewLocation = MouseLandLocation;
	Tracer::FixLocationToGrid(NewLocation, GetStepSize());

	if (bIsStartFixed)
	{
		// Rotate according to relative position of MousePosition
		// Extend points according to the relative location of MousePosition
	}
	else
	{
		SetActorLocation(NewLocation);
	}

	return false;
}
// ---------------------------------------------------------------


const bool ARoadSpline::CanAfford(const FResourceVault& PlayerResources)
{
	return false;
}
// ---------------------------------------------------------------

*/

