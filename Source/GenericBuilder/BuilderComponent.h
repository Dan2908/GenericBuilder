// Author: Danilo Brandolin
// 11/13/2023

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "Interface/Buildable.h"

#include "BuilderComponent.generated.h"

class ABuilderPlayerState;
class ABuilderPlayerController;
class AGenericBuilderGameModeBase;
class ABuilderPlayerPawn;

struct FResourceVault;
struct FBuildingAssetInfo;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GENERICBUILDER_API UBuilderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Constructor
	UBuilderComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Setup the preview for the given Building info. Return true if the preview was succesful.
	UFUNCTION(BlueprintCallable)
	const bool StartPreview(TSubclassOf<AActor> BuildingClass, const int BuildingID);

	// Restart preview with a new preview building.
	UFUNCTION(BlueprintCallable)
	void RestartPreview();

	// Stops the current preview.
	UFUNCTION(BlueprintCallable)
	void StopPreview();

	// Call this to rotate the building (Yaw) if there is one currently being held.
	void RotatePreview(const float DeltaYaw);

	// Call this to actually place the building in the world.
	const bool ConfirmBuilding();

	// Checks whether the component is currently previewing.
	inline const bool IsPreviewing() const;

	// Gets the current preview IBuildable pointer.
	inline IBuildable* GetPreview();

	// Updates the building preview position and aspect if is currently previewing.
	void HandlePreview(const ABuilderPlayerController& Controller);


private:

	// Auxiliar Game Mode pointer
	AGenericBuilderGameModeBase* CurrentGameMode;

	// Current preview Buildable
	IBuildable* Preview;
	// Current preview class.
	TSubclassOf<AActor> PreviewClass;

	// Max distance difference between corners to consider buildable terrain
	UPROPERTY(EditAnywhere, Category = "Building Rules")
	float MaxCornerDifference = 200.0f;

	// Get Current Auth GameMode.
	inline AGenericBuilderGameModeBase* GetGameMode();

	// Get the owning player of this component.
	inline ABuilderPlayerPawn* GetOwningPlayer();

	// Get the owning player's state.
	inline ABuilderPlayerState* GetPlayerState();
	
};
