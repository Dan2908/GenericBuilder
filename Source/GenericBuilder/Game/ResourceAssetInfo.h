// Author: Danilo Brandolin

#pragma once

#include "CoreMinimal.h"
#include "Definitions/GameAssetInfo.h"

#include "ResourceAssetInfo.generated.h"

// The counting method of a resource. Used for wrapping the ResourceValue
UENUM(BlueprintType)
enum EResourceCount
{
	Integer,
	Boolean
};


/**
* Resource Representation is a ligthweight structure holding the basic data to easily assign
* transaction data depending on the Resource collection. For example building costs.
*/
USTRUCT(BlueprintType)
struct FResourceRep
{
	GENERATED_BODY()

	FResourceRep() : Name(""), Value(0) {}

	FResourceRep(const FString& InName)
		: FResourceRep()
	{
		Name = InName;
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString Name;

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

	// Resource Count Type.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Data")
	TEnumAsByte<EResourceCount> CountType;
	// Resource amount
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Resource Data")
	int Count;

};
