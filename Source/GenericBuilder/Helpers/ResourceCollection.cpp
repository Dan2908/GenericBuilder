// Author: Danilo Brandolin

#include "Helpers/ResourceCollection.h"


// Constructor
UResourceCollection::UResourceCollection()
{
}

inline const TArray<FResourceType>& UResourceCollection::GetResourceList() const
{
	return ResourceList;
}
// ---------------------------------------------------------------

// Gets a collection of resource representations. The order of the resources is the collection's original.
inline void UResourceCollection::FillDefaultResourceData(FResourceData& OutResourceData)
{
	//Ensure is empty
	OutResourceData.Empty();

	for (auto& Element : ResourceList)
	{
		OutResourceData.Push(FResourceRep(Element.Name));
	}
}
// ---------------------------------------------------------------

//Finds the resource index in the collection for the given name.
//Returns Resource index if found, -1 if not found.
inline const int UResourceCollection::FindResourceIndex(FString Name) const
{
	for (int i = 0; i < ResourceList.Num(); ++i)
	{
		if (Name.Compare(ResourceList[i].Name) == 0)
		{
			return i;
		}
	}

	return -1;
}
// ---------------------------------------------------------------
