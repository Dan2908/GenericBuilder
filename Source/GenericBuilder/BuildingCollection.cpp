// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingCollection.h"

#include "BaseBuilding.h"
#include "Engine/Texture2D.h"

// Constructor
UBuildingCollection::UBuildingCollection()
{
	// Sort buildings array
	SortByID(1, false);
}
// ---------------------------------------------------------------


// Recursive swap sort for building array
void UBuildingCollection::SortByID(const int Index, bool Changed)
{
	// Reached end
	if (Index >= AvailableBuildings.Num())
	{
		if (Changed)
		{
			SortByID(1, Changed);
		}
			
		return;

	}

	// Change when element N is less than element N-1
	Changed = (AvailableBuildings[Index].Identifier < AvailableBuildings[Index - 1].Identifier);

	if (Changed)
	{
		AvailableBuildings.Swap(Index, Index - 1);
	}
	// Check next element.
	SortByID(Index + 1, Changed);

}
// ---------------------------------------------------------------

inline const bool FBuildingInformation::operator==(TSubclassOf<ABaseBuilding>& OtherBuilding)
{
	return OtherBuilding == BaseBuilding;
}