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

// Update const values when property has changed.
void UBuildingCollection::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const bool AddedArray = PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayAdd;

	// if a new building is added in the editor, fill IDs and the cost array for it.
	if (AddedArray)
	{
		const int Index = Buildings.Num() - 1;
		check(Buildings.IsValidIndex(Index));

		InitializeIDs();
	}

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
