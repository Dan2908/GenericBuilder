// Author: Danilo Brandolin
// 11/13/2023

#pragma once

#include "BaseBuilding.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GenericBuilderGameModeBase.h"
#include "BuilderPlayerController.h"
#include "Interface/Buildable.h"

#include "BuilderComponent.generated.h"

class ABuilderPlayerState;
struct FResourceVault;
struct FBuildingAssetInfo;

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

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Takes any location and relocate the held building to the closest grid step subdivision
	UFUNCTION(BlueprintCallable)
	void AdjustPreviewLocation(FVector CloseLocation);

	// Setup the preview for the given Building info. Return true if the preview was succesful.
	UFUNCTION(BlueprintCallable)
	const bool StartPreview(TSubclassOf<AActor> BuildingClass, const int BuildingID);

	// Restart preview with a new preview building.
	UFUNCTION(BlueprintCallable)
	void RestartPreview();

	// Stops the current preview.
	// Returns the left building pointer if any.
	UFUNCTION(BlueprintCallable)
	void StopPreview();

	// Call this to rotate the building (Yaw) if there is one currently being held.
	void RotatePreview(const float DeltaYaw);

	// Call this to actually place the building in the world.
	const bool ConfirmBuilding();

	// Checks whether the component is currently previewing.
	inline const bool IsPreviewing() const { return Preview != nullptr; }

	inline IBuildable* GetPreview()
	{
		return Preview;
	}

	// Updates the building preview position and aspect if is currently previewing.
	void HandlePreview(const ABuilderPlayerController& Controller);


private:

	AGenericBuilderGameModeBase* CurrentGameMode;

	IBuildable* Preview;
	TSubclassOf<AActor> PreviewClass;

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
