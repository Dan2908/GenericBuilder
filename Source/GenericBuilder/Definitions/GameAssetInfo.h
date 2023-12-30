// Author: Danilo Brandolin

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "GameAssetInfo.generated.h"

USTRUCT(BlueprintType)
struct FGameAssetInfo
{
	GENERATED_BODY()

	// Asset Name.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Data")
	FString Name;
	// Asset ID.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Basic Data")
	int ID;
	// Thumbnail Texture.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Data")
	TObjectPtr<UTexture2D> Thumbnail;

	FORCEINLINE bool operator==(const FGameAssetInfo& Other) const
	{
		return (Name.Compare(Other.Name) == 0);
	}

	FORCEINLINE bool operator<(const FGameAssetInfo& Other) const
	{
		return (Name.Compare(Other.Name) < 0);
	}


};
