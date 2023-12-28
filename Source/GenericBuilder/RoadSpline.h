// Author: Danilo Brandolin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Buildable.h"

#include "RoadSpline.generated.h"

class USplineComponent;

UCLASS()
class GENERICBUILDER_API ARoadSpline : public AActor, public IBuildable
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
	// IBuildable
	UFUNCTION(BlueprintCallable, Category = "Buildable Interface")
	const bool HandleMouseMove(const FVector MouseLandLocation);

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
	void Confirm() 
	{}

	UFUNCTION()
	void Cancel()
	{}

protected:
	// Gets the calculated extents for this buildable
	UFUNCTION(BlueprintNativeEvent, Category = "Buildable Interface")
	inline FVector2D GetExtents() const;
	// Get Construction Cost for this building from the current available buildings from the current collection.
	// Return null if can't be found.
	UFUNCTION(BlueprintNativeEvent, Category = "Buildable Interface")
	const FResourceVault* GetConstructionCost();

private:

	// Mesh of the road to be used through the spline
	UPROPERTY(EditAnywhere, Category = "Spline Settings")
	TSubclassOf<UStaticMesh> RoadMesh;

	// Minimal road tile extent. this means how long is a segment of road.
	UPROPERTY(EditAnywhere, Category = "Spline Settings")
	float TileExtent = 20.0f;

	// Adjust spline point locations to the ground
	void ProjectSplinePointsToGround();

	bool bIsStartFixed = false;


};
