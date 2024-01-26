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
	// Constructor
	FResourceVault();
	// Array of resources
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FResourceValue> Resources;
	// Adds the Amount of ResourceType to this Vault.
	inline void AddResource(const EGB_Resources ResourceType, const float Amount);
	// Adds the given Resource to this Vault.
	inline void AddResource(const FResourceValue& Resource);
	// Returns true if the resources in this vault are enough to pay the CostArray. False otherwise.
	// Remaining array is filled with the result of applying the cost to this vault.
	const bool GetPreviewedPayment(const TArray<FResourceValue>& CostArray, TArray<FResourceValue>& Preview);
	// Returns true if the resources in this vault are enough to pay the CostArray. False otherwise.
	// Note: cheaper function than GetPreviewedPayment to quickly get if can afford.
	const bool CanAfford(const TArray<FResourceValue>& CostArray) const;
	// Force set the resources to the given NewValues.
	void SetResources(const TArray<FResourceValue>& NewValues);

private:
	// Initialize Vault with and according to static definitions.
	inline void InitializeVault();

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

	// Make sure the resources correspond to the EGB_Resources enum
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	// Gets resource collection reference
	UFUNCTION(BlueprintCallable)
	const TArray<FResourceAssetInfo>& GetResources() const;

private:

	// Array containing the resource information
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collection", meta = (AllowPrivateAccess = "true"))
	TArray<FResourceAssetInfo> Resources;
};