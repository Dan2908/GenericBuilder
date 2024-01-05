// Author: Danilo Brandolin
// 11/13/2023


#include "BuilderPlayerPawn.h"

#include "BaseBuilding.h"
#include "BuilderComponent.h"
#include "BuilderPlayerController.h"
#include "BuilderPlayerState.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GenericBuilderGameModeBase.h"
#include "Helpers/BuilderInputCollection.h"
#include "Interface/Buildable.h"


// Sets default values
ABuilderPlayerPawn::ABuilderPlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MainCapsule = CreateDefaultSubobject<UCapsuleComponent>(FName("Main Capsule"));
	RootComponent = MainCapsule;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	Camera->SetupAttachment(SpringArm);

	BuilderComponent = CreateDefaultSubobject<UBuilderComponent>(FName("Building Component"));

}
// ---------------------------------------------------------------

// Called every frame
void ABuilderPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(MyController->GetControlMode() == EGB_ControlMode::BuildMode)
	{
		BuilderComponent->HandlePreview(*MyController);
	}

	// Update Production Task
	BuilderPlayerState->UpdateProductionCount(DeltaTime);

}
// ---------------------------------------------------------------

// Called to bind functionality to input
void ABuilderPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	InputCollection = NewObject<UBuilderInputCollection>(this, InputCollectionClass);
	
	ensure(InputCollection);

	InputCollection->StartInputObject(PlayerInputComponent, Controller);

	// Set up action bindings
	InputCollection->BindTriggerAction(this, InputCollection->MoveAction, &ABuilderPlayerPawn::Move);
	InputCollection->BindTriggerAction(this, InputCollection->RotateAction, &ABuilderPlayerPawn::Rotate);
	InputCollection->BindTriggerAction(this, InputCollection->ZoomAction, &ABuilderPlayerPawn::Zoom);
	InputCollection->BindTriggerAction(this, InputCollection->RotateHeldBuildingAction, &ABuilderPlayerPawn::RotateHeldBuilding);
	InputCollection->BindTriggerAction(this, InputCollection->ConfirmAction, &ABuilderPlayerPawn::Confirm);
	InputCollection->BindTriggerAction(this, InputCollection->EscapeAction, &ABuilderPlayerPawn::Escape);

}
// ---------------------------------------------------------------

// Call this from UI to check and set building controller
void ABuilderPlayerPawn::CallBuildMode(const FBuildingAssetInfo& SelectedBuilding)
{
	if (BuilderComponent->StartPreview(SelectedBuilding.BaseBuilding, SelectedBuilding.ID))
	{
		MyController->SetBuildMode();
	}
	
}
// ---------------------------------------------------------------

// Called when the game starts or when spawned
void ABuilderPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	MyController = CastChecked<ABuilderPlayerController>(Controller);

	AddZoom(0);

	BuilderPlayerState = CastChecked<ABuilderPlayerState>(GetPlayerState());

}

// Move the pawn always snapped to the landscape
void ABuilderPlayerPawn::Move(const FInputActionValue& Value)
{
	const FVector2D InputAxis = CalculateDeltaSpeed(Value.Get<FVector2D>(), CameraSpeed);

	const FVector DeltaForward = GetActorForwardVector() * InputAxis.X;
	const FVector DeltaRight = GetActorRightVector() * InputAxis.Y;
	const FVector DeltaUp = GetActorUpVector() * GetFloorOffset();

	AddActorWorldOffset(DeltaForward, true);
	AddActorWorldOffset(DeltaRight, true);
	AddActorWorldOffset(DeltaUp, true);
}
// ---------------------------------------------------------------

// Rotate the pawn along the Z axis (the camera rotates around the target view point)
void ABuilderPlayerPawn::Rotate(const FInputActionValue& Value)
{
	const float DeltaAngle = CalculateDeltaSpeed(Value.Get<float>(), CameraRotationSpeed);

	const FQuat DeltaRotation = FQuat(GetActorUpVector(), DeltaAngle);

	AddActorWorldRotation(DeltaRotation, true);
}
// ---------------------------------------------------------------

// Get the camera closer or further away from the view point
void ABuilderPlayerPawn::Zoom(const FInputActionValue& Value)
{
	const float DeltaZoom = CalculateDeltaSpeed(Value.Get<float>(), CameraZoomSpeed);

	AddZoom(DeltaZoom);
}
// ---------------------------------------------------------------

// Rotate the currently held building.
void ABuilderPlayerPawn::RotateHeldBuilding(const FInputActionValue& Value)
{
	const float Yaw = CalculateDeltaSpeed(Value.Get<float>(), HeldBuildingRotSpeed);

	BuilderComponent->RotatePreview(Yaw);

	UE_LOG(LogTemp, Display, TEXT("Rotation: %f"), Yaw);
}
// ---------------------------------------------------------------

// Confirm building, removal, etc
void ABuilderPlayerPawn::Confirm(const FInputActionValue& Value)
{
	if (!MyController->IsInBuildingMode())
	{
		return;
	}

	BuilderComponent->ConfirmBuilding();

}
// ---------------------------------------------------------------

// Confirm building, removal, etc
void ABuilderPlayerPawn::Escape(const FInputActionValue& Value)
{
	// Call Component to leave the building
	BuilderComponent->StopPreview();

	// Set default control mode
	MyController->SetDefaultMode();

}
// ---------------------------------------------------------------

// Safely add zoom by varying Spring Arm's lenght. Bound into CameraZoomMinMax.
void ABuilderPlayerPawn::AddZoom(const float DeltaZoom)
{
	float& ArmLength = SpringArm->TargetArmLength;
	// Clamp value within Min and Maz zoom
	ArmLength = FMath::Clamp(ArmLength + DeltaZoom, CameraZoomMinMax.X, CameraZoomMinMax.Y);
}
// ---------------------------------------------------------------

// Gets the height (Z) this camera is from the floor, tracing a vertical line to the
// floor. Meant to use when player is moving (X, Y), the Z component must be calculated
// following the landscape height in that point.
const float ABuilderPlayerPawn::GetFloorOffset()
{
	FHitResult Hit;
	// Get a point up and a point down the pawn
	const FVector UpPoint = GetActorLocation() +  FVector::UpVector * 1000;
	const FVector BottomPoint = GetActorLocation() -  FVector::UpVector * 1000;

	// Trace with "Landscape" channel
	if (!GetWorld()->LineTraceSingleByChannel(Hit, UpPoint, BottomPoint, ECollisionChannel::ECC_GameTraceChannel1))
	{
		return 0.0f;
	}

	return Hit.Location.Z - GetActorLocation().Z;
}
// ---------------------------------------------------------------


