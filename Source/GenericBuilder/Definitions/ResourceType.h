// Author: Danilo Brandolin


#pragma once

#include "CoreMinimal.h"
#include "ResourceType.generated.h"

// The counting method of a resource. Used for wrapping the ResourceValue
UENUM(BlueprintType)
enum EResourceCountType
{
	Integer,
	Boolean
};

USTRUCT(BlueprintType)
struct FResourceType
{
	GENERATED_BODY()

	// Resource name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Data")
	FString Name;
	// Resource Count Type.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Data")
	TEnumAsByte<EResourceCountType> CountType;
	// Display Thumbnail 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Data")
	UTexture2D* Thumbnail;
	// Resource amount
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Resource Data")
	int Count;

};