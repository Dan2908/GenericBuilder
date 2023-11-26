// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Definitions/Buildings.h"
#include "Definitions/BuildingTypes.h"

#include "BuildingCollection.generated.h"

class ABaseBuilding;
class UTexture2D;

/**
 * Structure that holds in Game details and information of the building.
 */
USTRUCT(BlueprintType)
struct FBuildingInformation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Attributes")
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Attributes")
	TEnumAsByte<EBuildingID> Identifier;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building Attributes")
	TEnumAsByte<EBuildingType> Type;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building Attributes")
	float ConstructionCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Attributes")
	UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Attributes")
	TSubclassOf<ABaseBuilding> BaseBuilding;

	inline const bool operator==(TSubclassOf<ABaseBuilding>& OtherBuilding);

};

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

	// List of buildings available in this collection
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buildings Available")
	TArray<FBuildingInformation> AvailableBuildings;

private:

	// TODO: Check if necessary, the idea is to find a way to look for information quicky, but the TArray Find method could be good for this case.
	// Recursive swap sort for building array
	void SortByID(const int Index, bool Changed);
};
