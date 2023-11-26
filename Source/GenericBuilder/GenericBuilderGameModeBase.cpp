// Copyright Epic Games, Inc. All Rights Reserved.


#include "GenericBuilderGameModeBase.h"

#include "BaseBuilding.h"
#include "BuilderPlayerPawn.h"
#include "BuildingCollection.h"

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

const TArray<FBuildingInformation> AGenericBuilderGameModeBase::GetAvailableBuildings()
{
	return BuildingCollection.GetDefaultObject()->AvailableBuildings;

}
