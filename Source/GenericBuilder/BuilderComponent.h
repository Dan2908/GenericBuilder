// Author: Danilo Brandolin
// 11/13/2023

#pragma once

#include "BaseBuilding.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GenericBuilderGameModeBase.h"

#include "BuilderComponent.generated.h"

class ABuilderPlayerState;
struct FResourceVault;
struct FBuildingAssetInfo;
class IBuildable;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GENERICBUILDER_API UBuilderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuilderComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Set Building aspect to indicate if the placement is ok
	void SetPlacementAspect(const bool IsPlacing, const bool IsGoodLocation);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Takes any location and relocate the held building to the closest grid step subdivision
	UFUNCTION(BlueprintCallable)
	void AdjustPreviewLocation(FVector CloseLocation);

	// Setup the preview for the given Building info. Return true if the preview was succesful.
	UFUNCTION(BlueprintCallable)
	const bool StartPreview(const FBuildingAssetInfo& SelectedBuilding);

	// Restart preview with a new preview building.
	UFUNCTION(BlueprintCallable)
	void RestartPreview();

	// Stops the current preview.
	// Returns the left building pointer if any.
	UFUNCTION(BlueprintCallable)
	ABaseBuilding* StopPreview();

	// Make the checkings and returns if the building can be built in the pointed world location.
	UFUNCTION(BlueprintCallable)
	const bool CanBuildHere();

	// Sets the building aspect according to the correct placing in the world
	UFUNCTION(BlueprintCallable)
	void SetBuildingAspect(const bool PlaceOK);
	
	// Call this to rotate the building (Yaw) if there is one currently being held.
	void RotatePreview(const float DeltaYaw);

	// Call this to actually place the building in the world.
	// Note: This function does not check placement, check CanBuildHere() before call this.
	const bool ConfirmBuilding();

	// Returns true if any other building or obstacle is overlapping the desired building location.
	inline const bool IsPlaceObstructed() const;

	// Checks whether the component is currently previewing.
	inline const bool IsPreviewing() const { return PreviewBuilding != nullptr; }

	inline IBuildable* GetPreview()
	{
		return Preview;
	}


private:

	const FBuildingAssetInfo* PreviewBuildingInfo;
	// Preview Building
	ABaseBuilding* PreviewBuilding;
	// Resource Vault preview of what are the resources after appliyng the building cost.
	FResourceVault* PreviewRemainingResources;

	AGenericBuilderGameModeBase* CurrentGameMode;

	IBuildable* Preview;

	float GridUnitSize;
	float StepSize;

	// Max distance difference between corners to consider buildable terrain
	UPROPERTY(EditAnywhere, Category = "Building Rules")
	float MaxCornerDifference = 200.0f;

	inline AGenericBuilderGameModeBase* GetGameMode() { return Cast<AGenericBuilderGameModeBase>(GetWorld()->GetAuthGameMode()); }

	inline ABuilderPlayerPawn* GetOwningPlayer();

	inline ABuilderPlayerState* GetPlayerState();

	// Get Location adjusted to map grid.
	inline void GetRoundedLocation(FVector& WorldLocation);
	
};
