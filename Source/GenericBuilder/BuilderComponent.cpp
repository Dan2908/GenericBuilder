// Author: Danilo Brandolin
// 11/13/2023


#include "BuilderComponent.h"

#include "BaseBuilding.h"
#include "BuilderPlayerController.h"
#include "BuilderPlayerPawn.h"
#include "BuilderPlayerState.h"
#include "Components/BoxComponent.h"
#include "Game/BuildingAssetInfo.h"
#include "GenericBuilderGameModeBase.h"
#include "Helpers/Tracer.h"
#include "Kismet/GameplayStatics.h"


// Constructor
UBuilderComponent::UBuilderComponent()
	: CurrentGameMode(nullptr)
	, Preview(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;

}
// ---------------------------------------------------------------

// Called when the game starts
void UBuilderComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentGameMode = GetGameMode();

	check(CurrentGameMode != nullptr);
}
// ---------------------------------------------------------------

// Called every frame
void UBuilderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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

// Get Current Auth GameMode.
inline AGenericBuilderGameModeBase* UBuilderComponent::GetGameMode() 
{ 
	return Cast<AGenericBuilderGameModeBase>(GetWorld()->GetAuthGameMode()); 
}
// ---------------------------------------------------------------

// Get the owning player of this component.
inline ABuilderPlayerPawn* UBuilderComponent::GetOwningPlayer()
{ 
	return Cast<ABuilderPlayerPawn>(GetOwner()); 
}
// ---------------------------------------------------------------

// Get the owning player's state.
inline ABuilderPlayerState* UBuilderComponent::GetPlayerState()
{
	return Cast<ABuilderPlayerState>(GetOwningPlayer()->GetPlayerState());
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

// Checks whether the component is currently previewing.
inline const bool UBuilderComponent::IsPreviewing() const 
{
	return Preview != nullptr;
}
// ---------------------------------------------------------------

// Gets the current preview IBuildable pointer.
inline IBuildable* UBuilderComponent::GetPreview()
{
	return Preview;
}
// ---------------------------------------------------------------

// Updates the building preview position and aspect if is currently previewing.
void UBuilderComponent::HandlePreview(const ABuilderPlayerController& Controller)
{
	if (!IsPreviewing())
	{
		return;
	}

	FVector LocationInLand;

	if (Controller.GetMouseLocationInGrid(LocationInLand, CurrentGameMode->GetStepSize()))
	{
		const bool CanBuild = Preview->CanBuild(GetPlayerState());

		Preview->MoveBuildable(LocationInLand);

		Preview->SetPreviewAspect(CanBuild);

	}

}
// ---------------------------------------------------------------