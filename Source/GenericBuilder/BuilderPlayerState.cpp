// Author: Danilo Brandolin

#include "BuilderPlayerState.h"

#include "BaseBuilding.h"
#include "Game/ResourceCollection.h"
#include "Interface/Buildable.h"
#include "ProductionBuilding.h"
#include "RoadSpline.h"
#include "Tasks/Task.h"


// Checks if a payment can be done with the current PlayerResources. The OutCalculation is filled with the resources remaining.
// Returns true if all resources can be paid, false if at least 1 is not enough.
const bool ABuilderPlayerState::Pay(const TArray<FResourceValue>& CostArray)
{
	TArray<FResourceValue> ResultingResources;
	if (PlayerResources.GetPreviewedPayment(CostArray, ResultingResources))
	{
		PlayerResources.SetResources(ResultingResources);
		return true;
	}

	return false;
}
// ---------------------------------------------------------------

// Register a buildingto  belong to this player
void ABuilderPlayerState::RegisterBuilding(IBuildable* RegisterNewBuilding, const EGB_BuildableType BuildingType)
{
	const bool BuildingAlreadyExists = PlayerBuildings.Find(RegisterNewBuilding) != INDEX_NONE;
	if (BuildingAlreadyExists)
	{
		return;
	}
	// Push new building
	PlayerBuildings.Push(RegisterNewBuilding);
	// Push as Production Building
	AProductionBuilding* AsProductionBuilding = Cast<AProductionBuilding>(RegisterNewBuilding);
	if (AsProductionBuilding && BuildingType == EGB_BuildableType::Production)
	{
		ProductionBuildings.Push(AsProductionBuilding);

	}

}
// ---------------------------------------------------------------

// Unregister a building that belongs to this player. Returns false if OurBuilding is not found in this player's list.
const bool ABuilderPlayerState::UnregisterBuilding(IBuildable* OurBuilding)
{
	int32 Index = PlayerBuildings.Find(OurBuilding);

	if (Index == INDEX_NONE)
	{
		return false;
	}
	// Remove from the list.
	PlayerBuildings.RemoveAt(Index);

	// Remove from production list.
	AProductionBuilding* AsProductionBuilding = Cast<AProductionBuilding>(OurBuilding);
	Index = ProductionBuildings.Find(AsProductionBuilding);

	if (Index != INDEX_NONE)
	{
	ProductionBuildings.RemoveAt(Index);
	}

	return false;
}
// ---------------------------------------------------------------

// Calls UpdateProductionTask() when accummulated ProductionUpdateCounter reaches ProductionUpdateTime.
void ABuilderPlayerState::UpdateProductionCount(float DeltaTime)
{
	ProductionUpdateCounter += DeltaTime;
	// if counter didn't reach the time threshold, skip
	if (ProductionUpdateCounter < ProductionUpdateTime)
	{
		return;
	}

	// If last task didn't finish yet, dont restart counter (ideally this may never happen)
	if (ProductionTask.IsCompleted())
	{
		ProductionUpdateCounter = 0.0f;
		ProductionTask = UE::Tasks::Launch(UE_SOURCE_LOCATION, [this] { UpdateProductionTask(); });
	}

}
// ---------------------------------------------------------------

// Updates the production stock for every Production building of this player.
void ABuilderPlayerState::UpdateProductionTask()
{
	for (AProductionBuilding* Building : ProductionBuildings)
	{
		if (Building->bBuildingActive)
		{
			Building->RefreshStock();
		}
	}

}
// ---------------------------------------------------------------

// Retrieves Production task.
inline UE::Tasks::FTask& ABuilderPlayerState::GetProductionTask()
{
	return ProductionTask;
}
// ---------------------------------------------------------------
