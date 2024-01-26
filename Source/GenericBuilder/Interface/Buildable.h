// Author: Danilo Brandolin

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "Buildable.generated.h"

class AGenericBuilderGameModeBase;
class ABuilderPlayerState;
struct FResourceVault;
struct FBuildingAssetInfo;
struct FPreviewMaterialSettings;

// --------------------------
// Enum EGB_BuildableType
// --------------------------

UENUM(BlueprintType)
enum EGB_BuildableType
{
	Residential,
	Production,
	Storage,
	Road
};

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

// Aspect when the placement is OK
static const FPreviewMaterialSettings GoodLocationAppearance = FPreviewMaterialSettings(FLinearColor::Green, 0.5f);
// Aspect when the placement is not OK
static const FPreviewMaterialSettings BadLocationAppearance = FPreviewMaterialSettings(FLinearColor::Red, 0.5f);
// Default Aspect
static const FPreviewMaterialSettings DefaultAppearance = FPreviewMaterialSettings(FLinearColor::White, 1.0f);

// --------------------------
// IBuildable
// --------------------------

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBuildable : public UInterface
{
	GENERATED_BODY()

public:
};

/**
 * 
 */
class GENERICBUILDER_API IBuildable
{
	GENERATED_BODY()

public:

	// Buildable type from EGB_BuildableType enum.
	TEnumAsByte<EGB_BuildableType> BuildableType;

	// Custom buildable ID
	int BuildableID = 0;

	// Move this buildable over the land following the user cursor.
	UFUNCTION(Category = "Buildable Interface")
	virtual void MoveBuildable(const FVector NewLocation) {}
	
	// Rotate this building by DeltaYaw.
	virtual void RotateBuildable(const float DeltaYaw) {}

	// Checks if this buildable can be afford by the player
	virtual const bool CanAfford(const FResourceVault& PlayerResources) { return false; }

	// Checks if this buildable is obstructed in the current location
	virtual const bool IsObstructed() { return true; }

	// Checks if the land under building is correct
	virtual const bool IsLandRight() { return false; }

	// Get Construction Cost for this building from the current available buildings from the current collection.
	virtual const FResourceVault* GetConstructionCost() { return nullptr; }

	// Gets the calculated extents for this buildable
	virtual FVector2D GetExtents() const { return FVector2D(); }

	// TODO: Find a system to fetch the can't build reasons
	// Return true if CanAfford, IsObstructed and IsLandRight evaluate to true. False if at least one is false.
	const bool CanBuild(ABuilderPlayerState* PlayerState);

	// Use DMIs to set preview aspect according to CanBuild.
	inline void SetPreviewAspect(const bool CanBuild);

	// Back to normal material aspect, when is no longer a preview.
	inline void SetNormalAspect();

protected:

	// Gets the available buildings for the current building collection.
	const TArray<FBuildingAssetInfo>* GetAvailableBuildings();

	// Generate Dynamic Material Instances from existing mesh materials. Returns true if all materials could be created.
	const bool GeneratePreviewMaterials(UStaticMeshComponent* Mesh);

	// Sets the building appearance, this is used to mainly to create previews and tweak colors. 
	inline void SetMaterialAspect(const FPreviewMaterialSettings MaterialInfo);

	// Tries to get builder game mode from the current object's world. Returns null if not found.
	inline const AGenericBuilderGameModeBase* FetchBuilderGM() const;
private:


	// Dynamic Material Instances
	TArray<class UMaterialInstanceDynamic*> DMIs;
};
