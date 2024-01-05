// Author: Danilo Brandolin

#pragma once

#include "CoreMinimal.h"
#include "Definitions/GameAssetInfo.h"
#include "Interface/Buildable.h"
#include "ResourceAssetInfo.h"
#include "ResourceCollection.h"

#include "BuildingAssetInfo.generated.h"

class ABaseBuilding;
class UResourceCollection;

/**
* Basic Structure holding asset data for buildings.
*/
USTRUCT(BlueprintType)
struct FBuildingAssetInfo : public FGameAssetInfo
{
	GENERATED_BODY()

	// ABaseBuilding associated with this building.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Attributes")
	TSubclassOf<AActor> BaseBuilding;
	// An array of value pairs (Resource Name/Value)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building Attributes")
	FResourceVault ConstructionCost;
};