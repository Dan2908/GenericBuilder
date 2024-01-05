// Author: Danilo Brandolin

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Resources.generated.h"

//List of existing Resources.
UENUM(BlueprintType)
enum EGB_Resources
{
	Currency,
	Wood,
	Planks,
	Stone,
	Bricks,
	Iron,

	Count

};

// The counting method of a resource. Used for wrapping the Resource Value.
UENUM(BlueprintType)
enum EGB_ResourceCount
{
	Integer,
	Float,
	Boolean
};

// Type of resource production.
UENUM(BlueprintType)
enum EGB_ResourceProductionType
{
	Extracted,				// Directly from environment.
	Manufactured,			// Produced from Raw resources.
	CurrencyProduction		// Currency (Does somebody really knows how banks works?)
};

/**
* A resource representation structure holding the basic Resource/Value data.
*/
USTRUCT(BlueprintType)
struct FResourceValue
{
	GENERATED_BODY()

	FResourceValue()
		: FResourceValue(Currency, 0)
	{}

	FResourceValue(EGB_Resources ID, const int Value = 0)
		: ResourceID(ID)
		, Value(Value)
	{}

	// Indicates wich resource is this. From EGB_Resources.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<EGB_Resources> ResourceID = EGB_Resources::Currency;

	// Amount of this resource.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double Value;

	inline const bool operator==(const EGB_Resources ID)
	{
		return ResourceID == ID;
	}

};
