// Author: Danilo Brandolin
// 11/13/2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "BuilderPlayerController.generated.h"

class UBuilderComponent;
class ABaseBuilding;

// Control mode
UENUM()
enum EGB_ControlMode : uint8
{
	Default, 
	BuildMode, 
	BuildRoadMode
};

/**
 Player controller for builder.
*/
UCLASS()
class GENERICBUILDER_API ABuilderPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	// Gets The currento controllerMode
	UFUNCTION(BlueprintCallable)
	inline TEnumAsByte<EGB_ControlMode> GetControlMode();

	// Sets control to "Building Mode"
	UFUNCTION(BlueprintCallable)
	void SetBuildMode();

	// Sets control to "Build Road Mode"
	UFUNCTION(BlueprintCallable)
	void SetBuildRoadMode();

	// Sets control to "Default Mode"
	UFUNCTION(BlueprintCallable)
	void SetDefaultMode();

	// Get where in the world is the player mouse pointing at.
	UFUNCTION(BlueprintCallable)
	const bool GetMouseLocationInGrid(FVector& Location, const float StepSize) const;

	// Returns true if controller is in building mode. False otherwise.
	UFUNCTION(BlueprintCallable)
	const bool IsInBuildingMode();

private:

	EGB_ControlMode ControlMode = EGB_ControlMode::Default;

};
