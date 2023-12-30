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

	AActor* PreviewActor = CastChecked<AActor>(Preview);
	PreviewActor->SetActorLocation(CloseLocation);

}
// ---------------------------------------------------------------

// Setup the preview for the given Building info. Return true if the preview was succesfully started, false otherwise.
const bool UBuilderComponent::StartPreview(TSubclassOf<AActor> BuildingClass, const int BuildingID)
{
	// Create Preview
	PreviewClass = BuildingClass;

	Preview = Cast<IBuildable>(GetWorld()->SpawnActor(BuildingClass));

	if (Preview)
	{
		Preview->BuildableID = BuildingID;
	}

	return Preview != nullptr;
}
// ---------------------------------------------------------------


// Restart preview with a new preview.
void UBuilderComponent::RestartPreview()
{
	StartPreview(PreviewClass, Preview->BuildableID);

}
// ---------------------------------------------------------------

// Stops the current preview.
void UBuilderComponent::StopPreview()
{
	// Edge: No building to leave
	if (!IsPreviewing())
	{
		return;
	}

	if (AActor* AsActor = Cast<AActor>(Preview))
	{
		GetWorld()->DestroyActor(AsActor);
	}

	PreviewClass = nullptr;
	Preview = nullptr;
	
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

// Call this to rotate the building (Yaw) if there is one currently being held.
void UBuilderComponent::RotatePreview(const float DeltaYaw)
{
	if (IsPreviewing())
	{
		Preview->RotateBuildable(DeltaYaw);
	}
	
}
// ---------------------------------------------------------------

// Call this to actually place the building in the world.
const bool UBuilderComponent::ConfirmBuilding()
{
	if (!IsPreviewing() || !Preview->CanBuild(GetPlayerState()))
	{
		return false;
	}

	if (ABuilderPlayerState* PState = GetPlayerState())
	{
		PState->Pay(Preview->GetConstructionCost()->Resources);
		PState->RegisterBuilding(Preview, Preview->BuildableType);

		Preview->SetNormalAspect();

		RestartPreview();
		return true;
	}

	return false;

}
// ---------------------------------------------------------------
// 
// Updates the building preview position and aspect if is currently previewing.
void UBuilderComponent::HandlePreview(const ABuilderPlayerController& Controller)
{
	if (!IsPreviewing())
	{
		return;
	}

	FVector LocationInLand;

	AGenericBuilderGameModeBase* GameMode = GetGameMode();
	ABuilderPlayerState* PlayerState = GetPlayerState();

	if (GameMode && Controller.GetGridedMouseLocation(LocationInLand, GameMode))
	{
		const bool CanBuild = Preview->CanBuild(PlayerState);
		Preview->SetPreviewAspect(CanBuild);
		Preview->HandleMouseMove(LocationInLand);
	}

}
// ---------------------------------------------------------------