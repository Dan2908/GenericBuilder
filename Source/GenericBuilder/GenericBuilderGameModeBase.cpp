// Author: Danilo Brandolin

#include "GenericBuilderGameModeBase.h"

#include "BaseBuilding.h"
#include "BuilderPlayerPawn.h"
#include "Game/BuildingCollection.h"
#include "Game/ResourceCollection.h"

// Adds the given target building to the game building list
void AGenericBuilderGameModeBase::RegisterNewBuilding(IBuildable* Target)
{
	BuildingList.Add(Target);
}
// ---------------------------------------------------------------

// Removes the given building from the game building list.
void AGenericBuilderGameModeBase::RemoveBuilding(IBuildable* Target)
{
	BuildingList.Remove(Target);
}
// ---------------------------------------------------------------

// Gets the index of the given player in the player list. Returns -1 if not found
inline const int AGenericBuilderGameModeBase::GetPlayerIndex(ABuilderPlayerPawn* Player) const
{
	const int* Result = PlayerList.Find(Player);
	return (Result ? *Result : -1 );
}
// ---------------------------------------------------------------

// Gets the available buildings list for this game mode.
const TArray<FBuildingAssetInfo>& AGenericBuilderGameModeBase::GetAvailableBuildings()
{
	return BuildingCollection.GetDefaultObject()->GetBuildings();

}
// ---------------------------------------------------------------

// Gets the available resources list for this game mode.
const TArray<FResourceAssetInfo>& AGenericBuilderGameModeBase::GetAvailableResources()
{
	return ResourceCollection.GetDefaultObject()->GetResources();
}
// ---------------------------------------------------------------

// Gets Default Resource Collection set in this game mode.
// Resturns nullptr if not set
inline UResourceCollection* AGenericBuilderGameModeBase::GetDefaultResourceCollection()
{
	return ResourceCollection.GetDefaultObject();

}
// ---------------------------------------------------------------
