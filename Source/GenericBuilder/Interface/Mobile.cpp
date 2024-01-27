// Author: Danilo Brandolin

#include "Interface/Mobile.h"

// Returns true if this mobile is currently moving.
inline const bool IMobile::IsMoving()
{
	return bIsMoving;
}

// Sets this mobile to move. If there are no more points to follow it keeps stopped and returns false.
inline const bool IMobile::StartMoving()
{
	if (!bIsMoving && !NextPoint.IsEmpty())
	{
		bIsMoving = true;
	}

	return bIsMoving;
}

// Sets this mobile to stop moving.
inline void IMobile::StopMoving()
{
	bIsMoving = false;
}

// Call this function in a Tick() to update this mobile movement.
inline void IMobile::UpdateLocation(const float DeltaTime, const FVector CurrentActorLocation)
{
	if (bIsMoving)
	{
		Advance(DeltaTime);
		CheckPoint(CurrentActorLocation);
	}
}

// This function checks whether the next target point has been reach and sets the target to the next point if any.
// Once all the points available has been reach, sets the internal bIsMoving to false.
inline void IMobile::CheckPoint(const FVector CurrentActorLocation)
{
	if (!NextPoint.IsEmpty())
	{
		if ((NextPoint[0] - CurrentActorLocation).IsNearlyZero())
		{
			NextPoint.RemoveAt(0);
		}
	}
	else
	{
		StopMoving();
	}
}
