// Author: Danilo Brandolin

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ResourceAssetInfo.h"

#include "ResourceCollection.generated.h"


// ---------------------------------------------------------------
//  FResourceVault
// ---------------------------------------------------------------

// Vault with minimal references to the current resources system
// Resources constants are defined in ResourceAssetInfo.h EGB_Resources
USTRUCT(BlueprintType)
struct FResourceVault
{
	GENERATED_BODY()
	// Array of resources
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FResourceRep> Resources;
	// Constructor
	FResourceVault();
	// Initialize Vault with and according to static definitions.
	inline void InitializeVault();
	// Preview applying given a given cost. Returns true if the stored resources are enough.
	// Remaining is filled with the result of applying the cost to this vault.
	const bool PreviewCost(const TArray<FResourceRep>& Cost, TArray<FResourceRep>& Remaining) const;
};

// ---------------------------------------------------------------
//  ResourceCollection
// ---------------------------------------------------------------

UCLASS(Blueprintable)
class GENERICBUILDER_API UResourceCollection : public UObject
{
	GENERATED_BODY()

public:
	// Constructor
	UResourceCollection();

	// Gets resource collection reference
	UFUNCTION()
	const TArray<FResourceAssetInfo>& GetResources() const;

protected:

	// Called when a property on this object has been modified externally
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

private:

	// Array containing the resource information
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collection", meta = (AllowPrivateAccess = "true"))
	TArray<FResourceAssetInfo> Resources;
};