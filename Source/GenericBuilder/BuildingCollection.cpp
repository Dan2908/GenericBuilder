// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingCollection.h"

#include "Helpers/ResourceCollection.h"
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
	const bool AddedBuilding = AddedArray && PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UBuildingCollection, AvailableBuildings);

	// if a new building is added in the editor, fill IDs and the cost array for it.
	if (AddedBuilding)
	{
		InitializeCostArray(AvailableBuildings.Num() - 1);
		InitializeIDs();
	}

}
// ---------------------------------------------------------------

// Fills the cost array for the building specified. With error print in case it fails.
void UBuildingCollection::InitializeCostArray(const int Index)
{
	if (!AvailableBuildings.IsValidIndex(Index))
	{
		UE_LOG(LogTemp, Warning, TEXT("UBuildingCollection::FillCostArray - Invalid Building index %d"), Index);
		return;
	}

	if (UResourceCollection* Resources = ResourceCollection.GetDefaultObject())
	{
		Resources->FillDefaultResourceData(AvailableBuildings[Index].ConstructionCost);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UBuildingCollection::FillCostArray - No resource collection found!"));
	}

}
// ---------------------------------------------------------------

// Initializes all the building IDs, corresponding to its index in the array
void UBuildingCollection::InitializeIDs()
{
	// Set IDs
	int ID = 0;
	for (FBuildingDetails& Details : AvailableBuildings)
	{
		Details.Identifier = ID;
		++ID;
	}
}
// ---------------------------------------------------------------
