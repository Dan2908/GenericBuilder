// Author: Danilo Brandolin
// 11/13/2023

#include "BaseBuilding.h"

#include "Brushes/SlateImageBrush.h"
#include "Game/BuildingCollection.h"
#include "Engine/Texture2D.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Helpers/Tracer.h"
#include "GenericBuilderGameModeBase.h"


// Sets default values
ABaseBuilding::ABaseBuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Building Mesh"));
	RootComponent = Mesh;

}
// ---------------------------------------------------------------

// Called every frame
void ABaseBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
// ---------------------------------------------------------------

// Get Construction Cost for this building from the current available buildings from the current collection.
const FResourceVault* ABaseBuilding::GetConstructionCost()
{
	if (const TArray<FBuildingAssetInfo>* BuildingCollection = GetAvailableBuildings())
	{
		if(BuildingCollection->IsValidIndex(BuildableID))
		{
			return &(*BuildingCollection)[BuildableID].ConstructionCost;
		}

	}

	return nullptr;
}
// ---------------------------------------------------------------

// Gets the calculated extents for this buildable
inline FVector2D ABaseBuilding::GetExtents() const
{
	return Extents;
}
// ---------------------------------------------------------------

// Called when the game starts or when spawned
void ABaseBuilding::BeginPlay()
{
	Super::BeginPlay();

	GeneratePreviewMaterials(Mesh);
}
// ---------------------------------------------------------------


// ---------------------------------------------------------------
// IBuildable Definitions
// ---------------------------------------------------------------

// Move this buildable over the land following the user cursor.
const bool ABaseBuilding::HandleMouseMove(const FVector MouseLandLocation)
{
	FVector NewLocation = MouseLandLocation;
	// Fix location to 
	Tracer::FixLocationToGrid(NewLocation, GetStepSize());

	SetActorLocation(NewLocation);

	return false;
}
// ---------------------------------------------------------------

// Rotate this building by DeltaYaw.
void ABaseBuilding::RotateBuildable(const float DeltaYaw)
{
	FRotator Rot(0, DeltaYaw, 0);
	AddActorLocalRotation(Rot);

}
// ---------------------------------------------------------------

// Checks if this buildable can be afford by the player
const bool ABaseBuilding::CanAfford(const FResourceVault& PlayerResources)
{
	return PlayerResources.CanAfford(GetConstructionCost()->Resources);

}
// ---------------------------------------------------------------

// Checks if this buildable is obstructed in the current location
const bool ABaseBuilding::IsObstructed()
{
	TSet<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	return OverlappingActors.Num() > 0;
}
// ---------------------------------------------------------------

// Checks if the land under building is correct
const bool ABaseBuilding::IsLandRight()
{
	Tracer BuildTracer(GetWorld(), GetActorTransform(), Extents.X, Extents.Y);

	// Correct preview z position
	const FVector PreviewLocation = GetActorLocation() * FVector(1, 1, 0) + FVector(0, 0, BuildTracer.GetHighestCorner());

	return BuildTracer.GetCornersDiff() < MaxCornerDifference;
}
// ---------------------------------------------------------------