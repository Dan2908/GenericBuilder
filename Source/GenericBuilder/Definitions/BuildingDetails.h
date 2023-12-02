// Author: Danilo Brandolin

#pragma once

#include "CoreMinimal.h"
#include "BaseBuilding.h"
#include "ResourceType.h"
#include "Helpers/ResourceCollection.h"

#include "BuildingDetails.generated.h"

class ABaseBuilding;

UENUM(BlueprintType)
enum EBuildingType
{
	Residential,
	Production,
	Storage
};

/**
 * Structure that holds in Game details and information of the building.
 */
USTRUCT(BlueprintType)
struct FBuildingDetails
{
	GENERATED_BODY()
	// Cost Array, a collection of resources needed depending on the game mode.
	typedef TArray<FResourceRep> FResourceData;

	FBuildingDetails();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Attributes")
	FName Name;
	// ID
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Building Attributes")
	int Identifier;
	// Building type
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building Attributes")
	TEnumAsByte<EBuildingType> Type;
	// An array of value pairs (Resource Name/Value)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building Attributes")
	TArray<FResourceRep> ConstructionCost;
	// Image use for thumbnails of this building
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Attributes")
	UTexture2D* Thumbnail;
	// ABaseBuilding associated with this building.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Attributes")
	TSubclassOf<ABaseBuilding> BaseBuilding;

};