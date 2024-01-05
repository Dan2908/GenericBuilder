// Author: Danilo Brandolin
// 11/13/2023

#include "BuilderPlayerController.h"

#include "BuilderComponent.h"
#include "GenericBuilderGameModeBase.h"
#include "Helpers/Tracer.h"
#include "Interface/Buildable.h"

// Gets The currento controllerMode
inline TEnumAsByte<EGB_ControlMode> ABuilderPlayerController::GetControlMode()
{
	return ControlMode;
}
// ---------------------------------------------------------------

// Sets control to "Default Mode"
void ABuilderPlayerController::SetDefaultMode()
{
	ControlMode = EGB_ControlMode::Default;
	// Show cursor
	SetShowMouseCursor(true);

}
// ---------------------------------------------------------------

// Get where in the world is the player mouse pointing at adjusted to the grid.
const bool ABuilderPlayerController::GetMouseLocationInGrid(FVector& Location, const float StepSize) const
{
	FVector Direction;
	DeprojectMousePositionToWorld(Location, Direction);


	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, Location, Location + Direction * 3000, ECollisionChannel::ECC_GameTraceChannel1))
	{
		Location = Hit.Location;
		Tracer::FixLocationToGrid(Location, StepSize);
		return true;
	}

	
	return false;
}
// ---------------------------------------------------------------

// Returns true if controller is in building mode. False otherwise.
const bool ABuilderPlayerController::IsInBuildingMode()
{
	return GetControlMode() == EGB_ControlMode::BuildMode;
}
// ---------------------------------------------------------------

// Sets control to "Building Mode"
void ABuilderPlayerController::SetBuildMode()
{
	// Set build mode
	ControlMode = EGB_ControlMode::BuildMode;
	// Hide cursor
	SetShowMouseCursor(false);
}
// ---------------------------------------------------------------

// Sets control to "Build Road Mode"
void ABuilderPlayerController::SetBuildRoadMode()
{
	// Set build mode
	ControlMode = EGB_ControlMode::BuildRoadMode;
	// Hide cursor
	SetShowMouseCursor(false);
}
// ---------------------------------------------------------------

