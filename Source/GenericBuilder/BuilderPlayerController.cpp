// Author: Danilo Brandolin
// 11/13/2023


#include "BuilderPlayerController.h"

#include "BuilderComponent.h"

// Gets The currento controllerMode
inline TEnumAsByte<EControlMode> ABuilderPlayerController::GetControlMode()
{
	return ControlMode;
}
// ---------------------------------------------------------------

// Sets control to "Default Mode"
void ABuilderPlayerController::SetDefaultMode()
{
	ControlMode = EControlMode::Default;
	// Show cursor
	SetShowMouseCursor(true);

}
// ---------------------------------------------------------------

// Get where in the world is the player mouse pointing at.
const bool ABuilderPlayerController::GetMouseLocationInLand(FVector& Location) const
{
	FHitResult Hit;
	FVector Direction;

	DeprojectMousePositionToWorld(Location, Direction);

	if (GetWorld()->LineTraceSingleByChannel(Hit, Location, Location + Direction * 3000, ECollisionChannel::ECC_GameTraceChannel1))
	{
		Location = Hit.Location;
		return true;
	}
	
	return false;
}

// Returns true if controller is in building mode. False otherwise.
const bool ABuilderPlayerController::IsInBuildingMode()
{
	return GetControlMode() == EControlMode::BuildMode;
}
// ---------------------------------------------------------------

// TODO: Correct this, mouse delta seems to always return 0.
// Check if the mouse movement wasn't nearly zero
const bool ABuilderPlayerController::GetDidNotMove()
{
	FVector2D MouseDelta;
	GetInputMouseDelta(MouseDelta.X, MouseDelta.Y);

	UE_LOG(LogTemp, Display, TEXT("MouseDelta.X = %f"), MouseDelta.X);
	UE_LOG(LogTemp, Display, TEXT("MouseDelta.Y = %f"), MouseDelta.Y);
	return FMath::IsNearlyZero(MouseDelta.X) && FMath::IsNearlyZero(MouseDelta.Y);
}
// ---------------------------------------------------------------

// Sets control to "Building Mode"
void ABuilderPlayerController::SetBuildMode()
{
	// Set build mode
	ControlMode = EControlMode::BuildMode;
	// Hide cursor
	SetShowMouseCursor(false);
}
// ---------------------------------------------------------------

