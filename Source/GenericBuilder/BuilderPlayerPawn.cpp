// Author: Danilo Brandolin
// 11/13/2023


#include "BuilderPlayerPawn.h"

#include "BuilderComponent.h"
#include "BaseBuilding.h"
#include "BuilderPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"

// ---------------------------------------------------------------
// -------------------------- PUBLIC --------------------------
// ---------------------------------------------------------------

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

	if(MyController->GetControlMode() == EControlMode::BuildMode)
	{
		HandleHeldBuilding();
	}

}
// ---------------------------------------------------------------

// Called to bind functionality to input
void ABuilderPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Move
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABuilderPlayerPawn::Move);
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ABuilderPlayerPawn::Rotate);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ABuilderPlayerPawn::Zoom);
		EnhancedInputComponent->BindAction(RotateHeldBuildingAction, ETriggerEvent::Triggered, this, &ABuilderPlayerPawn::RotateHeldBuilding);
		EnhancedInputComponent->BindAction(ConfirmAction, ETriggerEvent::Triggered, this, &ABuilderPlayerPawn::Confirm);
		EnhancedInputComponent->BindAction(EscapeAction, ETriggerEvent::Triggered, this, &ABuilderPlayerPawn::Escape);
	}
}
// ---------------------------------------------------------------

// Call this from UI to check and set building controller
void ABuilderPlayerPawn::CallBuildMode(UClass* BuildingClass)
{
	if (ABaseBuilding* NewBuilding = Cast<ABaseBuilding>(GetWorld()->SpawnActor(BuildingClass)))
	{
		// Make sure the new building is being held
		if (BuilderComponent->HoldBuilding(NewBuilding))
		{
			MyController->SetBuildMode();
		}
	}

}
// ---------------------------------------------------------------

// Do the logics to check and transform currently held building
void ABuilderPlayerPawn::HandleHeldBuilding()
{
	FVector Location;
	
	if (!MyController->GetMouseLocationInLand(Location))
	{
		return;
	}
	// Adjust the location to the grid.
	BuilderComponent->AdjustBuildingPosition(Location);

	const bool CanBuild = BuilderComponent->GetCanBuildHere();
	
	BuilderComponent->SetBuildingAspect(CanBuild);

}
// ---------------------------------------------------------------


// ---------------------------------------------------------------
// -------------------------- PROTECTED --------------------------
// ---------------------------------------------------------------

// Called when the game starts or when spawned
void ABuilderPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	MyController = CastChecked<ABuilderPlayerController>(Controller);

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}

	// Make sure Zoom is inbounds
	AddZoom(0);

}
// ---------------------------------------------------------------

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

	BuilderComponent->RotateBuilding(Yaw);

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

	if (BuilderComponent->GetCanBuildHere())
	{
		if (BuilderComponent->ConfirmBuilding())
		{
			// Leave Building reference
			ABaseBuilding* BuildingRef = BuilderComponent->LeaveBuilding();
			// Call build mode again with a copy of the building.
			CallBuildMode(BuildingRef->GetClass());
		}
	}

}
// ---------------------------------------------------------------


// Confirm building, removal, etc
void ABuilderPlayerPawn::Escape(const FInputActionValue& Value)
{
	// Call Component to leave the building
	if (ABaseBuilding* Discarded = BuilderComponent->LeaveBuilding())
	{
		// Destroy discarded building
		GetWorld()->DestroyActor(Discarded);
	}

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
	const FVector AbovePoint = GetActorLocation() + FVector::UpVector * 1000;
	// Trace with "Landscape" channel
	if (!GetWorld()->LineTraceSingleByChannel(Hit, AbovePoint, -AbovePoint, ECollisionChannel::ECC_GameTraceChannel1))
	{
		return 0.0f;
	}

	return Hit.Location.Z - GetActorLocation().Z;
}
// ---------------------------------------------------------------


