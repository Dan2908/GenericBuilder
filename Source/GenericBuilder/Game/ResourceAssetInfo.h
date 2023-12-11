// Author: Danilo Brandolin

#pragma once

#include "CoreMinimal.h"
#include "Definitions/GameAssetInfo.h"

#include "ResourceAssetInfo.generated.h"

// The counting method of a resource. Used for wrapping the ResourceValue
UENUM(BlueprintType)
enum EGB_ResourceCount
{
	Integer,
	Boolean
};


UENUM(BlueprintType)
enum EGB_Resources
{
	Currency,
	Wood,
	Stone,
	Bricks,
	Iron,

	Count

};

/**
* Resource Representation is a ligthweight structure holding the basic data to easily assign
* transaction data depending on the Resource collection. For example building costs.
*/
USTRUCT(BlueprintType)
struct FResourceRep
{
	GENERATED_BODY()

	FResourceRep() 
		: FResourceRep(Currency, 0)
	{}

	FResourceRep(EGB_Resources ID, const int Value = 0)
		: ID(ID)
		, Value(Value)
	{}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TEnumAsByte<EGB_Resources> ID = EGB_Resources::Currency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Value;
};


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
