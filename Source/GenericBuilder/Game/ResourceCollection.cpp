// Author: Danilo Brandolin

#include "ResourceCollection.h"


// Gets a collection of ResourseRep. The order of the resources is the collection's original.
inline void UResourceCollection::FillDefaultResourceData(ResourceData& OutResourceData)
{
	//Ensure is empty
	OutResourceData.Empty();

	for (const FResourceAssetInfo& Element : GetResources())
	{
		OutResourceData.Push(FResourceRep(Element.Name));
	}
}
// ---------------------------------------------------------------

// Gets resource collection reference
const TArray<FResourceAssetInfo>& UResourceCollection::GetResources() const
{
	return Resources;
}
// ---------------------------------------------------------------
