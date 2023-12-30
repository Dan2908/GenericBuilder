// Author: Danilo Brandolin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Buildable.h"

#include "RoadSpline.generated.h"

class USplineComponent;
struct FResourceVault;

UCLASS()
class GENERICBUILDER_API ARoadSpline : public AActor //, public IBuildable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoadSpline();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	USplineComponent* SplineComponent;

public:	

	// Extend road one point forward 
	UFUNCTION(BlueprintCallable)
	void ExtendRoad();

	// Decrease road one point backward 
	UFUNCTION(BlueprintCallable)
	void DecreaseRoad();

	// Fixes the current starting point, if possible, to the landscape point.
	UFUNCTION()
	void FixStartPoint();


	UFUNCTION()
	void Cancel()
	{}

	/*
	// IBuildable
		// 
		// Move this buildable over the land following the user cursor.
	virtual const bool HandleMouseMove(const FVector MouseLandLocation) override;
	// Rotate this building by DeltaYaw.
	virtual void RotateBuildable(const float DeltaYaw) override;
	// Checks if this buildable can be afford by the player
	virtual const bool CanAfford(const FResourceVault& PlayerResources) override;
	// Checks if this buildable is obstructed in the current location
	virtual const bool IsObstructed() override;
	// Checks if the land under building is correct
	virtual const bool IsLandRight() override;
	// Confirm action to build.
	virtual bool Confirm() override;
	// Get Construction Cost for this building from the current available buildings from the current collection.
	virtual const FResourceVault& GetConstructionCost() override;
	// Gets the calculated extents for this buildable
	virtual inline FVector2D GetExtents() const override;
	*/

protected:

private:

	// Mesh of the road to be used through the spline
	UPROPERTY(EditAnywhere, Category = "Spline Settings")
	TSubclassOf<UStaticMesh> RoadMesh;

	// Minimal road tile extent. this means how long is a segment of road.
	UPROPERTY(EditAnywhere, Category = "Spline Settings")
	float TileExtent = 20.0f;

	// Adjust spline point locations to the ground
	void ProjectSplinePointsToGround()
	{
	}

	bool bIsStartFixed = false;


};
