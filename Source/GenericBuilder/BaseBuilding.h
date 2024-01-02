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

	UPROPERTY(EditAnywhere, Category = "Building Adjustments")
	bool bBuildingActive = true;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// IBuildable
	// 
	// Move this buildable over the land following the user cursor.
	virtual void MoveBuildable(const FVector NewLocation) override;
	// Rotate this building by DeltaYaw.
	virtual void RotateBuildable(const float DeltaYaw) override;
	// Checks if this buildable can be afford by the player
	virtual const bool CanAfford(const FResourceVault& PlayerResources) override;
	// Checks if this buildable is obstructed in the current location
	virtual const bool IsObstructed() override;
	// Checks if the land under building is correct
	virtual const bool IsLandRight() override;
	// Get Construction Cost for this building from the current available buildings from the current collection.
	virtual const FResourceVault* GetConstructionCost() override;
	// Gets the calculated extents for this buildable
	virtual inline FVector2D GetExtents() const override;

private:

	// Max distance difference between corners to consider buildable terrain
	UPROPERTY(EditAnywhere, Category = "Building Adjustments")
	float MaxCornerDifference = 200.0f;

	// Base mesh
	UPROPERTY(VisibleAnywhere, Category = "Building Adjustments")
	UStaticMeshComponent* Mesh;

	// Building Extents
	UPROPERTY(EditAnywhere, Category = "Building Adjustments", meta = (AllowPrivateAccess = "true"))
	FVector2D Extents;

};
