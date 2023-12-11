// Author: Danilo Brandolin

#pragma once

#include "CoreMinimal.h"
#include "Definitions/GameAssetInfo.h"
#include "ResourceAssetInfo.h"
#include "ResourceCollection.h"

#include "BuildingAssetInfo.generated.h"

class ABaseBuilding;
class UResourceCollection;

// Type of building
UENUM(BlueprintType)
enum EGB_BuildingTypes
{
	Residential,
	Production,
	Storage
};

/**
* Basic Structure holding asset data for buildings.
*/
USTRUCT(BlueprintType)
struct FBuildingAssetInfo : public FGameAssetInfo
{
	GENERATED_BODY()


	// Building type
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building Attributes")
	TEnumAsByte<EGB_BuildingTypes> Type;
	// ABaseBuilding associated with this building.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Attributes")
	TSubclassOf<ABaseBuilding> BaseBuilding;
	// An array of value pairs (Resource Name/Value)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building Attributes")
	FResourceVault ConstructionCost;
};