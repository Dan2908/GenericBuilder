// Author: Danilo Brandolin

#pragma once

#include "CoreMinimal.h"
#include "Definitions/GameAssetInfo.h"
#include "Definitions/Resources.h"

#include "ResourceAssetInfo.generated.h"


/**
* Basic Structure holding asset data for resources
*/
USTRUCT(BlueprintType)
struct FResourceAssetInfo : public FGameAssetInfo
{
	GENERATED_BODY()

	FResourceAssetInfo(const TEnumAsByte<EGB_Resources> ResourceID = TEnumAsByte<EGB_Resources>(0));
	// Resource Count Type.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource Data")
	TEnumAsByte<EGB_Resources> ResourceID;
	// Resource Count Type.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource Data")
	TEnumAsByte<EGB_ResourceCount> CountType;
	// Resource amount
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Resource Data")
	int Count;

};
