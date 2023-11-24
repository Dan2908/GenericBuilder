// Author: Danilo Brandolin
// 11/13/2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BuilderPlayerController.generated.h"

class UBuilderComponent;
class ABaseBuilding;

/**
 * 
 */

UENUM()
enum EControlMode : uint8
{
	Default, BuildMode
};

UCLASS()
class GENERICBUILDER_API ABuilderPlayerController : public APlayerController
{
	GENERATED_BODY()


public:

	// Gets The currento controllerMode
	UFUNCTION(BlueprintCallable)
	inline TEnumAsByte<EControlMode> GetControlMode();
	// Sets control to "Building Mode"
	UFUNCTION(BlueprintCallable)
	void SetBuildMode();
	// Sets control to "Default Mode"
	UFUNCTION(BlueprintCallable)
	void SetDefaultMode();
	// Get where in the world is the player mouse pointing at.
	UFUNCTION(BlueprintCallable)
	const bool GetMouseLocationInLand(FVector& Location) const;
	// Returns true if controller is in building mode. False otherwise.
	UFUNCTION(BlueprintCallable)
	const bool IsInBuildingMode();
	// Check if the mouse movement wasn't nearly zero
	const bool GetDidNotMove();

private:

	EControlMode ControlMode = EControlMode::Default;

};
