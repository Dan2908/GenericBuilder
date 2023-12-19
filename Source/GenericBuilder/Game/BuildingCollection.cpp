// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingCollection.h"

#include "ResourceCollection.h"
#include "BaseBuilding.h"
#include "Engine/Texture2D.h"

// Constructor
UBuildingCollection::UBuildingCollection()
{
}
// ---------------------------------------------------------------


// Gets buildings collection reference
const TArray<FBuildingAssetInfo>& UBuildingCollection::GetBuildings() const
{
	return Buildings;
}
// ---------------------------------------------------------------

// Initializes all the building IDs, corresponding to its index in the array
void UBuildingCollection::InitializeIDs()
{
	// Set IDs
	int ID = 0;
	for (FBuildingAssetInfo& Info : Buildings)
	{
		Info.ID = ID;
		++ID;
	}
}
// ---------------------------------------------------------------
