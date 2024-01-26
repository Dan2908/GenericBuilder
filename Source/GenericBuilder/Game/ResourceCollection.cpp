// Author: Danilo Brandolin

#include "ResourceCollection.h"

// ---------------------------------------------------------------
//  UResourceCollection
// ---------------------------------------------------------------

UResourceCollection::UResourceCollection()
{

}

// Make sure the resources correspond to the EGB_Resources enum
void UResourceCollection::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Resources.SetNum(EGB_Resources::Count);

	for (int i = 0; i < EGB_Resources::Count; ++i)
	{
		Resources[i].ResourceID = TEnumAsByte<EGB_Resources>(i);
	}

}
// ---------------------------------------------------------------

// Gets resource collection reference
const TArray<FResourceAssetInfo>& UResourceCollection::GetResources() const
{
	return Resources;
}
// ---------------------------------------------------------------


// ---------------------------------------------------------------
//  FResourceVault
// ---------------------------------------------------------------

// Constructor
FResourceVault::FResourceVault()
{
	InitializeVault();
}
// ---------------------------------------------------------------

// Adds the Amount of ResourceType to this Vault.
void FResourceVault::AddResource(const EGB_Resources ResourceType, const float Amount)
{
	Resources[TEnumAsByte<EGB_Resources>(ResourceType)].Value += Amount;

}
// ---------------------------------------------------------------

// Adds the given Resource to this Vault.
inline void FResourceVault::AddResource(const FResourceValue& Resource)
{
	AddResource(Resource.ResourceID, Resource.Value);

}
// ---------------------------------------------------------------


// Initialize Vault with and according to static definitions.
inline void FResourceVault::InitializeVault()
{
	Resources.Empty();

	for (int i = 0; i < EGB_Resources::Count; ++i)
	{
		// Push new resource 		
		Resources.Push(FResourceValue(TEnumAsByte<EGB_Resources>(i)));
	}

}
// ---------------------------------------------------------------

// Preview applying given a given cost. Returns true if the stored resources are enough.
// Remaining is filled with the result of applying the cost to this vault.
const bool FResourceVault::GetPreviewedPayment(const TArray<FResourceValue>& CostArray, TArray<FResourceValue>& Preview)
{
	bool Enough = true;

	Preview.Reserve(CostArray.Num());
	for (const FResourceValue& Cost : CostArray)
	{
		Preview.Push(FResourceValue(Cost.ResourceID, Resources[Cost.ResourceID].Value - Cost.Value));
		if (Preview.Last().Value < 0)
		{
			Enough = false;
		}
	}

	return Enough;
}
// ---------------------------------------------------------------

// Returns true if the resources in this vault are enough to pay the CostArray. False otherwise.
// Note: cheaper function than GetPreviewedPayment to quickly get if can afford.
const bool FResourceVault::CanAfford(const TArray<FResourceValue>& CostArray) const
{
	for (const FResourceValue& Cost : CostArray)
	{
		const float Remaining = Resources[Cost.ResourceID].Value - Cost.Value;
		if (Remaining < 0)
		{
			return false;
		}
	}

	return true;
}
// ---------------------------------------------------------------

// Force set the resources to the given NewValues.
void FResourceVault::SetResources(const TArray<FResourceValue>& NewValues)
{
	for (const FResourceValue& NewValue : NewValues)
	{
		Resources[NewValue.ResourceID].Value = NewValue.Value;
	}
}
// ---------------------------------------------------------------
