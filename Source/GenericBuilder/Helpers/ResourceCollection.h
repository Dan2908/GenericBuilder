// Author: Danilo Brandolin

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Definitions/ResourceType.h"

#include "ResourceCollection.generated.h"


/**
 * 
 */
UCLASS(Blueprintable)
class GENERICBUILDER_API UResourceCollection : public UObject
{
	GENERATED_BODY()
	
	typedef TArray<FResourceRep> FResourceData;

	//TArray of resource reps
public:

	// Constructor
	UResourceCollection();

	const TArray<FResourceType>& GetResourceList() const;

	// Gets a collection of resource representations. The order of the resources is the collection's original.
	inline void FillDefaultResourceData(FResourceData& OutResourceData);

	//Finds the resource index in the collection for the given name.
	//Returns Resource index if found, -1 if not found.
	const int FindResourceIndex(FString Name) const;

protected:
	// List of all the resources thet can be gathered/used as cost
	// For simplicity reasons, lets add here also the unique resourses (ie distinctions, achievements, etc), treated as boolean
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
	TArray<FResourceType> ResourceList;

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


