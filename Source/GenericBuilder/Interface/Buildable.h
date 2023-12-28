// Author: Danilo Brandolin

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "Buildable.generated.h"

class AGenericBuilderGameModeBase;
struct FResourceVault;
struct FBuildingAssetInfo;
class ABuilderPlayerState;

// Aspect when the placement is OK
static const FPreviewMaterialSettings GoodLocationAppearance;
// Aspect when the placement is not OK
static const FPreviewMaterialSettings BadLocationAppearance;
// Default Aspect
static const FPreviewMaterialSettings DefaultAppearance;

// --------------------------
// Struct FPreviewMaterialSettings
// --------------------------

USTRUCT(BlueprintType)
struct FPreviewMaterialSettings
{
	GENERATED_BODY()

	FPreviewMaterialSettings()
		: Color(FLinearColor::White)
		, Opacity(1)
	{}

	FPreviewMaterialSettings(const FLinearColor Color, const float Opacity)
		: Color(Color)
		, Opacity(Opacity)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dynamic Material Parameters")
	FLinearColor Color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dynamic Material Parameters")
	float Opacity;

};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBuildable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GENERICBUILDER_API IBuildable
{
	GENERATED_BODY()

public:

	// Move this buildable over the land following the user cursor.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Buildable Interface")
	const bool HandleMouseMove(const FVector MouseLandLocation);
	// Checks if this buildable can be afford by the player
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Buildable Interface")
	const bool CanAfford(const FResourceVault& PlayerResources);
	// Checks if this buildable is obstructed in the current location
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Buildable Interface")
	const bool IsObstructed();
	// Checks if the land under building is correct
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Buildable Interface")
	const bool IsLandRight();

protected:

	// Gets the calculated extents for this buildable
	UFUNCTION(BlueprintNativeEvent, Category = "Buildable Interface")
	inline FVector2D GetExtents() const;
	// Get Construction Cost for this building from the current available buildings from the current collection.
	// Return null if can't be found.
	UFUNCTION(BlueprintNativeEvent, Category = "Buildable Interface")
	const FResourceVault* GetConstructionCost();


	// Gets the available buildings for the current building collection
	const TArray<FBuildingAssetInfo>& GetAvailableBuildings();

	// Generate Dynamic Material Instances from existing mesh materials. Returns true if all materials could be created.
	const bool GeneratePreviewMaterials(UStaticMeshComponent* Mesh);

	// Sets the building appearance, this is used to mainly to create previews and tweak colors. 
	inline void SetMaterialAspect(const FPreviewMaterialSettings MaterialInfo);
	
	// Check if can be built and use DMIs to set preview aspect
	inline void CheckAndSetAspect(ABuilderPlayerState* PlayerState);


private:

	// Tries to get builder game mode from the current object's world. Returns null if not found.
	inline AGenericBuilderGameModeBase* FetchBuilderGM();

	// Building Extents
	UPROPERTY(EditAnywhere, Category = "Building Adjustments", meta = (AllowPrivateAccess = "true"))
	FVector2D Extents;

	// Dynamic Material Instances
	TArray<class UMaterialInstanceDynamic*> DMIs;
};
