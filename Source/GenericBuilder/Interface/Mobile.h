// Author: Danilo Brandolin

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Mobile.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMobile : public UInterface
{
	GENERATED_BODY()
};

/**
 * This interface manages the moving of mobile entities. 
 * Must implement FindPath() and Advance() methods to calculate the movement.
 */
class GENERICBUILDER_API IMobile
{
	GENERATED_BODY()

public:

	// Finds a path to Target and returns true if found.
	// If the target is found, this method may store the next steps in NextPoint variable.
	virtual const bool FindPath(const FVector Target) = 0;

	// Returns true if this mobile is currently moving.
	inline const bool IsMoving();

	// Sets this mobile to move. If there are no more points to follow it keeps stopped and returns false.
	inline const bool StartMoving();

	// Sets this mobile to stop moving.
	inline void StopMoving();

	// Call this function in a Tick() to update this mobile movement.
	inline void UpdateLocation(const float DeltaTime, const FVector CurrentActorLocation);

protected:

	// This method implements the transforms to move this mobile.
	virtual void Advance(const float DeltaTime) = 0;

	// Holds 3D points as a itinerary of points this mobile should follow consecutively. 
	TArray<FVector> NextPoint;

	// Flag indicating if this mobile is currently moving.
	bool bIsMoving = false;

private:

	// This function checks whether the next target point has been reach and sets the target to the next point if any.
	// Once all the points available has been reach, sets the internal bIsMoving to false.
	inline void CheckPoint(const FVector CurrentActorLocation);
};
