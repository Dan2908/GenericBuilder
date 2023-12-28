// Author: Danilo Brandolin
// 11/13/2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/BuildingAssetInfo.h"
#include "Interface/Buildable.h"

#include "BaseBuilding.generated.h"

// ---------------------------------
// ------ CLASS ABaseBuilding ------
// ---------------------------------

UCLASS()
class GENERICBUILDER_API ABaseBuilding : public AActor, public IBuildable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseBuilding();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	TEnumAsByte<EGB_BuildingTypes> BaseBuildingType;

	UPROPERTY(EditAnywhere, Category = "Building Adjustments")
	bool bBuildingActive = true;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// IBuildable
	UFUNCTION(BlueprintCallable, Category = "Buildable Interface")
	const bool HandleMouseMove(const FVector MouseLandLocation);
	// Checks if this buildable can be afford by the player
	UFUNCTION(BlueprintCallable, Category = "Buildable Interface")
	const bool CanAfford(const FResourceVault& PlayerResources);
	// Checks if this buildable is obstructed in the current location
	UFUNCTION(BlueprintCallable, Category = "Buildable Interface")
	const bool IsObstructed();
	// Checks if the land under building is correct
	UFUNCTION(BlueprintCallable, Category = "Buildable Interface")
	const bool IsLandRight();

protected:

	// Get Construction Cost for this building from the current available buildings from the current collection.
	// Return null if can't be found.
	UFUNCTION(Category = "Buildable Interface")
	const FResourceVault* GetConstructionCost();
	// Gets the calculated extents for this buildable
	UFUNCTION(Category = "Buildable Interface")
	inline FVector2D GetExtents() const;


private:

	// Max distance difference between corners to consider buildable terrain
	UPROPERTY(EditAnywhere, Category = "Building Rules")
	float MaxCornerDifference = 200.0f;

	// Base mesh
	UPROPERTY(VisibleAnywhere, Category = "Building Adjustments")
	UStaticMeshComponent* Mesh;
	// Material Instance Dynamic
	TArray<class UMaterialInstanceDynamic*> MIDs;
	// Generate Material Instance Dynamics from existing materials
	void GenerateMIDs();

};
