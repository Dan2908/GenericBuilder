// Author: Danilo Brandolin


#include "RoadSpline.h"
#include "Helpers/Tracer.h"
#include "GenericBuilderGameModeBase.h"

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

const bool ARoadSpline::HandleMouseMove(const FVector MouseLandLocation)
{

	FVector ProjectedCursor = Tracer::TraceGround(this, MouseLandLocation);
	Tracer::RoundLocation(ProjectedCursor, GetBuilderGameMode()->GetStepSize());

	if (bIsStartFixed)
	{
		// Rotate according to relative position of MousePosition
		// Extend points according to the relative location of MousePosition
	}
	else
	{
		SetActorLocation(ProjectedCursor);
	}

	return false;
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



