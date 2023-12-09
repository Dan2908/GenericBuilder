// Copyright Epic Games, Inc. All Rights Reserved.


#include "GenericBuilderGameModeBase.h"

#include "BaseBuilding.h"
#include "BuilderPlayerPawn.h"
#include "Game/BuildingCollection.h"
#include "Game/ResourceCollection.h"


void AGenericBuilderGameModeBase::NewBuilding(ABaseBuilding* Target)
{
	BuildingList.Add(Target);
}

void AGenericBuilderGameModeBase::DestroyBuilding(ABaseBuilding* Target)
{
	BuildingList.Remove(Target);
}

inline const int AGenericBuilderGameModeBase::GetPlayerIndex(ABuilderPlayerPawn* Player) const
{
	const int* Result = PlayerList.Find(Player);
	return (Result ? *Result : -1 );
}

const TArray<FBuildingAssetInfo>& AGenericBuilderGameModeBase::GetAvailableBuildings()
{
	return BuildingCollection.GetDefaultObject()->GetBuildings();

}

const TArray<FResourceAssetInfo>& AGenericBuilderGameModeBase::GetAvailableResources()
{
	return ResourceCollection.GetDefaultObject()->GetResources();
}

// Gets Default Resource Collection set in this game mode.
// Resturns nullptr if not set
inline UResourceCollection* AGenericBuilderGameModeBase::GetDefaultResourceCollection()
{
	return ResourceCollection.GetDefaultObject();

}
