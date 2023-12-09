// Author: Danilo Brandolin

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Definitions/AssetCollection.h"
#include "ResourceAssetInfo.h"

#include "ResourceCollection.generated.h"


UCLASS(Blueprintable)
class GENERICBUILDER_API UResourceCollection : public UObject
{
	GENERATED_BODY()

	using ResourceData = TArray<FResourceRep>;

public:

	inline void FillDefaultResourceData(ResourceData& OutResourceData);

	// Gets resource collection reference
	UFUNCTION()
	const TArray<FResourceAssetInfo>& GetResources() const;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collection", meta = (AllowPrivateAccess = "true"))
	TArray<FResourceAssetInfo> Resources;
};