// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingCollection.h"

#include "BaseBuilding.h"
#include "Engine/Texture2D.h"
#include "ResourceCollection.h"

// Constructor
UBuildingCollection::UBuildingCollection()
{
	const int Size = Buildings.Num();
	for (int Index = 0; Index < Size; ++Index)
	{
		Buildings[Index].ID = Index++;
	}
}
// ---------------------------------------------------------------

// Gets const array of buildings in this collection.
const TArray<FBuildingAssetInfo>& UBuildingCollection::GetBuildings() const
{
	return Buildings;
}
// ---------------------------------------------------------------

// Called when a property on this object has been modified externally.
// @param PropertyThatChanged the property that was modified.
void UBuildingCollection::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	// Lambda function
	auto SetBuildingIDs = [this]()
		{
			const int Size = Buildings.Num();
			for (int Index = 0; Index < Size; ++Index)
			{
				Buildings[Index].ID = Index;
			}
		};

	// Change cases
	switch (PropertyChangedEvent.ChangeType)
	{
	case EPropertyChangeType::ArrayAdd:
	case EPropertyChangeType::ArrayMove:
	case EPropertyChangeType::ArrayRemove:
		SetBuildingIDs();
	};

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
// ---------------------------------------------------------------
