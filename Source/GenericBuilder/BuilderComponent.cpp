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
#include "Game/BuildingAssetInfo.h"
#include "BuilderPlayerState.h"

// Sets default values for this component's properties
UBuilderComponent::UBuilderComponent()
	: GridUnitSize(0)
	, StepSize(0)
	, PreviewBuilding(nullptr)
	, PreviewRemainingResources(new FResourceVault())

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

	PreviewBuilding->SetActorLocation(CloseLocation);

}
// ---------------------------------------------------------------

// Setup the preview for the given Building info. Return true if the preview was succesfully started, false otherwise.
const bool UBuilderComponent::StartPreview(const FBuildingAssetInfo& SelectedBuilding)
{
	// Create Preview
	if (ABaseBuilding* Preview = Cast<ABaseBuilding>(GetWorld()->SpawnActor(SelectedBuilding.BaseBuilding.Get())))
	{
		PreviewBuildingInfo = &SelectedBuilding;
		PreviewBuilding = Preview;

		return true;
	}

	UE_LOG(LogTemp, Error, TEXT("UBuilderComponent::StartPreview - Unable to start preview with the selected building."));

	return false;
}
// ---------------------------------------------------------------


// Restart preview with a new preview.
void UBuilderComponent::RestartPreview()
{
	StartPreview(*PreviewBuildingInfo);

}
// ---------------------------------------------------------------

// Stops the current preview.
// Returns the left building pointer if any.
ABaseBuilding* UBuilderComponent::StopPreview()
{
	// Edge: No building to leave
	if (!IsPreviewing())
	{
		return nullptr;
	}

	ABaseBuilding* Output = PreviewBuilding;

	PreviewBuildingInfo = nullptr;
	PreviewBuilding = nullptr;

	return Output;
	
}
// ---------------------------------------------------------------

// Make the checkings and returns if the building can be built in the pointed world location.
const bool UBuilderComponent::CanBuildHere()
{
	check(IsPreviewing());

	// Get Building scaled Extents
	const FVector2D& Extents = PreviewBuilding->GetXYExtents() * GridUnitSize;

	Tracer BuildTracer(GetWorld(), PreviewBuilding->GetActorTransform(), Extents.X, Extents.Y);

	// TODO: Do this elsewhere
	// Correct preview z position
	const FVector PreviewLocation = PreviewBuilding->GetActorLocation() * FVector(1, 1, 0) + FVector(0, 0, BuildTracer.GetHighestCorner());
	PreviewBuilding->SetActorLocation(PreviewLocation);

	const bool IsLandOk = BuildTracer.GetCornersDiff() < MaxCornerDifference;
	const bool IsObstructed = IsPlaceObstructed();
	const bool IsCostOk = GetPlayerState()->PlayerResources.CanAfford(PreviewBuildingInfo->ConstructionCost.Resources);

	if (!IsCostOk)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enougth resources."));
		return false;
	}

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

inline ABuilderPlayerPawn* UBuilderComponent::GetOwningPlayer() 
{ 
	return Cast<ABuilderPlayerPawn>(GetOwner()); 
}
// ---------------------------------------------------------------


inline ABuilderPlayerState* UBuilderComponent::GetPlayerState()
{
	return Cast<ABuilderPlayerState>(GetOwningPlayer()->GetPlayerState());
}
// ---------------------------------------------------------------

// Get Location adjusted to map grid.
inline void UBuilderComponent::GetRoundedLocation(FVector& WorldLocation)
{
	// TODO: Replace with Tracer function
	// Round XY values to the StepSize
	WorldLocation.X = ceil(WorldLocation.X / StepSize) * StepSize;
	WorldLocation.Y = ceil(WorldLocation.Y / StepSize) * StepSize;
}
// ---------------------------------------------------------------

// Sets the building aspect according to the correct placing in the world
void UBuilderComponent::SetBuildingAspect(const bool PlaceOK)
{
	PreviewBuilding->SetMaterialAspect(PlaceOK ? GoodLocationAppearance : BadLocationAppearance);
}
// ---------------------------------------------------------------

// Call this to rotate the building (Yaw) if there is one currently being held.
void UBuilderComponent::RotatePreview(const float DeltaYaw)
{
	if (PreviewBuilding)
	{
		FRotator Rot(0, DeltaYaw, 0);
		PreviewBuilding->AddActorLocalRotation(Rot);
		UE_LOG(LogTemp, Display, TEXT("Building %s Rotated %s"), *PreviewBuilding->GetName(), *Rot.ToString())
	}
}
// ---------------------------------------------------------------

// Call this to actually place the building in the world.
// Note: This function does not check placement, check CanBuildHere() before call this.
const bool UBuilderComponent::ConfirmBuilding()
{
	if (!IsPreviewing())
	{
		return false;
	}

	// Set to regular material.
	PreviewBuilding->SetMaterialAspect(DefaultAppearance);
	
	if (ABuilderPlayerState* PState = GetPlayerState())
	{
		PState->Pay(PreviewBuildingInfo->ConstructionCost.Resources);
		PState->RegisterBuilding(PreviewBuilding, PreviewBuilding->BaseBuildingType);

		RestartPreview();
		return true;
	}

	return false;

}


// Returns true if any other building or obstacle is overlapping the desired building location.
inline const bool UBuilderComponent::IsPlaceObstructed() const
{
	TSet<AActor*> Result;
	PreviewBuilding->GetOverlappingActors(Result);

	return Result.Num() > 0;
}
// ---------------------------------------------------------------

// Set Building aspect to indicate if the placement is ok
void UBuilderComponent::SetPlacementAspect(const bool IsPlacing, const bool IsGoodLocation)
{
	PreviewBuilding->SetMaterialAspect(!IsPlacing ? DefaultAppearance : (IsGoodLocation ? GoodLocationAppearance : BadLocationAppearance));
}
// ---------------------------------------------------------------
