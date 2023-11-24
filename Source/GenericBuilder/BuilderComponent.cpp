// Author: Danilo Brandolin
// 11/13/2023


#include "BuilderComponent.h"

#include "BuilderPlayerController.h"
#include "BaseBuilding.h"
#include "BuilderPlayerPawn.h"
#include "GenericBuilderGameModeBase.h"
#include <Kismet/GameplayStatics.h>
#include <Components/BoxComponent.h>
#include "Helpers/Tracer.h"
#include <set>


// Sets default values for this component's properties
UBuilderComponent::UBuilderComponent()
	: GridUnitSize(0)
	, StepSize(0)
	, HeldBuilding(nullptr)
	, PlacementBox()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UBuilderComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentGameMode = GetGameMode();
	GridUnitSize = CurrentGameMode->GetGridUnitSize();
	StepSize = CurrentGameMode->GetStepSize();
}
// ---------------------------------------------------------------

// Called every frame
void UBuilderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}
// ---------------------------------------------------------------

// Takes any location and relocate the held building to the closest grid step subdivision
void UBuilderComponent::AdjustPreviewLocation(FVector CloseLocation)
{
	GetRoundedLocation(CloseLocation);

	HeldBuilding->SetActorLocation(CloseLocation);

}
// ---------------------------------------------------------------

// Holds a new building to handle its positioning.
// Note: The object must be created outside.
const bool UBuilderComponent::HoldBuilding(ABaseBuilding* Building)
{
	HeldBuilding = Building;

	if (HeldBuilding)
	{
		// Get Extents
		FVector Extents(HeldBuilding->GetXYExtents() * GridUnitSize, 2);
		PlacementBox = FBox::BuildAABB(FVector::ZeroVector, Extents);
		
		return true;
	}

	return false;
}
// ---------------------------------------------------------------

// Leave the current held building. (Just deletes the reference, not the building itself).
// Returns the left building pointer if any.
ABaseBuilding* UBuilderComponent::LeaveBuilding()
{
	// Edge: No building to leave
	if (!IsHoldingBuilding())
	{
		return nullptr;
	}

	ABaseBuilding* Output = HeldBuilding;

	HeldBuilding = nullptr;

	return Output;
	
}
// ---------------------------------------------------------------

// Make the checkings and returns if the building can be built in the pointed world location.
const bool UBuilderComponent::GetCanBuildHere()
{
	check(IsHoldingBuilding());

	// Get Building scaled Extents
	const FVector Extents = PlacementBox.GetExtent();

	Tracer BuildTracer(GetWorld(), HeldBuilding->GetActorTransform(), Extents.X, Extents.Y);

	// TODO: Do this elsewhere
	// Correct preview z position
	const FVector PreviewLocation = HeldBuilding->GetActorLocation() * FVector(1, 1, 0) + FVector(0, 0, BuildTracer.GetHighestCorner());
	HeldBuilding->SetActorLocation(PreviewLocation);

	const bool IsLandOk = BuildTracer.GetCornersDiff() < MaxCornerDifference;
	const bool IsObstructed = IsPlaceObstructed();

	if (!IsLandOk)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't build here, bad land elevations."));
		return false;
	}

	if (IsObstructed)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't build here, building obstructed."));
		return false;
	}

	return true;
}
// ---------------------------------------------------------------

// Get Location adjusted to map grid.
inline void UBuilderComponent::GetRoundedLocation(FVector& WorldLocation)
{
	// Round XY values to the StepSize
	WorldLocation.X = ceil(WorldLocation.X / StepSize) * StepSize;
	WorldLocation.Y = ceil(WorldLocation.Y / StepSize) * StepSize;
}
// ---------------------------------------------------------------

// Sets the building aspect according to the correct placing in the world
void UBuilderComponent::SetBuildingAspect(const bool PlaceOK)
{
	HeldBuilding->SetMaterialAspect(PlaceOK ? GoodLocationAppearance : BadLocationAppearance);
}

// Call this to rotate the building (Yaw) if there is one currently being held.
void UBuilderComponent::RotateBuilding(const float DeltaYaw)
{
	if (HeldBuilding)
	{
		FRotator Rot(0, DeltaYaw, 0);
		HeldBuilding->AddActorLocalRotation(Rot);
		UE_LOG(LogTemp, Display, TEXT("Building %s Rotated %s"), *HeldBuilding->GetName(), *Rot.ToString())
	}
}
// ---------------------------------------------------------------

// Call this to actually place the building in the world.
// Note: This function does not check placement, check GetCanBuildHere() before call this.
const bool UBuilderComponent::ConfirmBuilding()
{
	if (!IsHoldingBuilding())
	{
		return false;
	}

	HeldBuilding->SetMaterialAspect(DefaultAppearance);
	// Set info and actually build in the world
	CurrentGameMode->NewBuilding(HeldBuilding);

	return true;

}
// ---------------------------------------------------------------

// Returns true if any other building or obstacle is overlapping the desired building location.
inline const bool UBuilderComponent::IsPlaceObstructed() const
{
	TSet<AActor*> Result;
	HeldBuilding->GetOverlappingActors(Result);

	return Result.Num() > 0;
}
// ---------------------------------------------------------------

// Set Building aspect to indicate if the placement is ok
void UBuilderComponent::SetPlacementAspect(const bool IsPlacing, const bool IsGoodLocation)
{
	HeldBuilding->SetMaterialAspect(!IsPlacing ? DefaultAppearance : (IsGoodLocation ? GoodLocationAppearance : BadLocationAppearance));
}
// ---------------------------------------------------------------
