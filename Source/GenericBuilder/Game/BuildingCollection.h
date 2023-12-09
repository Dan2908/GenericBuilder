// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Definitions/AssetCollection.h"
#include "Game/BuildingAssetInfo.h"

#include "BuildingCollection.generated.h"

class ABaseBuilding;
class UTexture2D;
class UResourceCollection;

/** 
	This object is meant to hold a list of building class references the can be assigned to a level/gamemode.
This list is auto constructed given a list of valid building assets, ordered by ID, providing simple method to access building attributes.
*/
UCLASS(Blueprintable)
class GENERICBUILDER_API UBuildingCollection : public UObject
{
	GENERATED_BODY()

public:

	// Constructor
	UBuildingCollection();
	// Update const values when property has changed.
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent);

	// Gets buildings collection reference
	UFUNCTION()
	const TArray<FBuildingAssetInfo>& GetBuildings() const;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collection", meta = (AllowPrivateAccess = "true"))
	TArray<FBuildingAssetInfo> Buildings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UResourceCollection> ResourceCollection;
	// Fills the cost array for the building specified. With error print in case it fails.
	void InitializeCostArray(const int Index);
	// Initializes all the building IDs, corresponding to its index in the array
	void InitializeIDs();

};
