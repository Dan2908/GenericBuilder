// Author: Danilo Brandolin


#include "RoadSpline.h"

#include "Components/SplineComponent.h"
#include "Game/ResourceCollection.h"
#include "GenericBuilderGameModeBase.h"
#include "Helpers/Tracer.h"

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