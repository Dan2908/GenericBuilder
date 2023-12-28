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
	TArray<FResourceValue> Resources;
	// Adds the Amount of ResourceType to this Vault.
	inline void AddResource(const EGB_Resources ResourceType, const float Amount);
	// Adds the given Resource to this Vault.
	inline void AddResource(const FResourceValue& Resource);
	// Constructor
	FResourceVault();
	// Initialize Vault with and according to static definitions.
	inline void InitializeVault();
	// Preview applying given a given cost. Returns true if the stored resources are enough.
	// Remaining is filled with the result of applying the cost to this vault.
	const bool GetPreviewedPayment(const TArray<FResourceValue>& CostArray, TArray<FResourceValue>& Preview);
	// Preview applying given a given cost. Returns true if the stored resources are enough.
	const bool CanAfford(const TArray<FResourceValue>& CostArray) const;

	void SetResources(const TArray<FResourceValue>& NewValues);

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

private:

	// Array containing the resource information
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collection", meta = (AllowPrivateAccess = "true"))
	TArray<FResourceAssetInfo> Resources;
};