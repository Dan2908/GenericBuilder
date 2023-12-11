#include "ResourceAssetInfo.h"

// Author: Danilo Brandolin

FResourceAssetInfo::FResourceAssetInfo(const TEnumAsByte<EGB_Resources> ResourceID)
{
	this->ResourceID = ResourceID;
}
