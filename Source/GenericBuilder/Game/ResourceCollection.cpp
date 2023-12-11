// Author: Danilo Brandolin

#include "ResourceCollection.h"


UResourceCollection::UResourceCollection()
{
}
// ---------------------------------------------------------------

// Gets resource collection reference
const TArray<FResourceAssetInfo>& UResourceCollection::GetResources() const
{
	return Resources;
}
// ---------------------------------------------------------------

// Called when a property on this object has been modified externally
void UResourceCollection::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

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

// Initialize Vault with and according to static definitions.
inline void FResourceVault::InitializeVault()
{
	if (Resources.Num() != EGB_Resources::Count)
	{
		Resources.Empty();

		for (int i = 0; i < EGB_Resources::Count; ++i)
		{
			// Push new resource
			Resources.Push(FResourceRep(TEnumAsByte<EGB_Resources>(i)));
		}
	}
}
// ---------------------------------------------------------------

// Preview applying given a given cost. Returns true if the stored resources are enough.
// Remaining is filled with the result of applying the cost to this vault.
const bool FResourceVault::PreviewCost(const TArray<FResourceRep>& Cost, TArray<FResourceRep>& Remaining) const
{
	const int ResourcesCount = Resources.Num();
	// Make sure cost have the same element numbers
	check(Cost.Num() == ResourcesCount);


	bool Enough = true;

	for (int i = 0; i < ResourcesCount; ++i)
	{
		const int ResultingValue = Resources[i].Value - Cost[i].Value;

		if (ResultingValue < 0)
		{
			Enough = false;
		}

		Remaining.Push(FResourceRep(Resources[i].ID, ResultingValue));

	}

	return Enough;
}
// ---------------------------------------------------------------
