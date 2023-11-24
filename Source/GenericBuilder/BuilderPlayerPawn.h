// Author: Danilo Brandolin
// 11/13/2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"

#include "BuilderPlayerPawn.generated.h"

class ABuilderPlayerController;
class ABaseBuilding;
class UBuilderInputCollection;


UCLASS()
class GENERICBUILDER_API ABuilderPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABuilderPlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Add zoom clamped to the min and max (this is spring arm length)
	void AddZoom(const float DeltaZoom);

	// Gets the height (Z) this camera is from the floor, tracing a vertical line to the
	// floor. Meant to use when player is moving (X, Y), the Z component must be calculated
	// following the landscape height in that point.
	const float GetFloorOffset();

	// Move the pawn snapping to the landscape
	void Move(const FInputActionValue& Value);
	// Rotate the pawn along the Z axis (the camera rotates around the target view point)
	void Rotate(const FInputActionValue& Value);
	// Get the camera closer or further away from the view point
	void Zoom(const FInputActionValue& Value);
	// Rotate the currently held building.
	void RotateHeldBuilding(const FInputActionValue& Value);
	// Confirm building, removal, etc
	void Confirm(const FInputActionValue& Value);
	// Confirm building, removal, etc
	void Escape(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Call this from UI to check and set building controller
	UFUNCTION(BlueprintCallable)
	void CallBuildMode(UClass* BuildingClass);

	UPROPERTY(EditAnywhere, Category = "Player Input")
	TSubclassOf<UBuilderInputCollection> InputCollectionClass;

private:

	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* MainCapsule;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class UBuilderComponent* BuilderComponent;

	UPROPERTY(EditAnywhere, Category = "Player Movement")
	float CameraSpeed = 800;

	UPROPERTY(EditAnywhere, Category = "Player Movement")
	float CameraRotationSpeed = 2;

	UPROPERTY(EditAnywhere, Category = "Player Movement")
	float CameraZoomSpeed = 500;

	UPROPERTY(EditAnywhere, Category = "Player Movement")
	FVector2D CameraZoomMinMax = FVector2D(300, 1500);

	UPROPERTY(EditAnywhere, Category = "Player Movement")
	float HeldBuildingRotSpeed = 200;
	// INPUT *****

	UPROPERTY(VisibleAnywhere, Category = "Player Input")
	UBuilderInputCollection* InputCollection;

	ABuilderPlayerController* MyController;

	// Do the logics to check and transform currently held building
	void HandleHeldBuilding();

	// Get the movement scaled by World's Delta seconds.
	template<typename T = float>
	inline const T CalculateDeltaSpeed(const T Value, const float InSpeed);
};

template<typename T>
inline const T ABuilderPlayerPawn::CalculateDeltaSpeed(const T Value, const float InSpeed)
{
	return Value * GetWorld()->GetDeltaSeconds() * InSpeed;
}
