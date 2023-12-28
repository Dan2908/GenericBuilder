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
// Return null if can't be found.
const FResourceVault* ABaseBuilding::GetConstructionCost()
{
	const TArray<FBuildingAssetInfo>& BuildingCollection = GetAvailableBuildings();
	const int Count = BuildingCollection.Num();

	for (int i = 0; i < Count; ++i)
	{
		if (BuildingCollection[i].BaseBuilding->StaticClass == StaticClass)
		{
			return &(BuildingCollection[i].ConstructionCost);
		}
	}

	return nullptr;
}
// ---------------------------------------------------------------

// Called when the game starts or when spawned
void ABaseBuilding::BeginPlay()
{
	Super::BeginPlay();
	
	GenerateMIDs();

}
// ---------------------------------------------------------------


// ---------------------------------------------------------------
// IBuildable Definitions
// ---------------------------------------------------------------

// Move this buildable over the land following the user cursor.
const bool ABaseBuilding::HandleMouseMove(const FVector MouseLandLocation)
{
	FVector ProjectedCursor = Tracer::TraceGround(this, MouseLandLocation);
	Tracer::RoundLocation(ProjectedCursor, GetBuilderGameMode()->GetStepSize());

	// Get Building scaled Extents
	const FVector2D& Extents = GetExtents() * GetBuilderGameMode()->GetGridUnitSize();

	Tracer BuildTracer(GetWorld(), GetActorTransform(), Extents.X, Extents.Y);

	// Correct preview z position
	const FVector PreviewLocation = GetActorLocation() * FVector(1, 1, 0) + FVector(0, 0, BuildTracer.GetHighestCorner());
	SetActorLocation(PreviewLocation);

	const bool IsLandOk = BuildTracer.GetCornersDiff() < MaxCornerDifference;

	TSet<AActor*> Result;
	GetOverlappingActors(Result);
	const bool IsObstructed = Result.Num() > 0;

	const bool IsCostOk = GetConstructionCost() != nullptr;
	//GetPlayerState()->PlayerResources.CanAfford(PreviewBuildingInfo->ConstructionCost.Resources);

	if (!IsCostOk)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enougth resources."));
		return false;
	}


	if (!IsLandOk)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't build here, bad land elevations."));
		return false;
	}

	if (IsObstructed)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't build here, building obstructed."));
		return false;
	}

	return false;
}
// ---------------------------------------------------------------

// Checks if this buildable can be afford by the player
const bool ABaseBuilding::CanAfford(const FResourceVault& PlayerResources)
{
	if (const FResourceVault* CostVault = GetConstructionCost())
	{
		return PlayerResources.CanAfford(CostVault->Resources);
	}

	return false;
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
	SetActorLocation(PreviewLocation);

	const bool IsLandOk = BuildTracer.GetCornersDiff() < MaxCornerDifference;
}
// ---------------------------------------------------------------

inline FVector2D ABaseBuilding::GetExtents() const
{
	return FVector2D();
}
// ---------------------------------------------------------------
