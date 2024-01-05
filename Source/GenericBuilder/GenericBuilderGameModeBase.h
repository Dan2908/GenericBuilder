// Author: Danilo Brandolin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "GenericBuilderGameModeBase.generated.h"

class ABaseBuilding;
class ABuilderPlayerPawn;
class ABuilderPlayerState;
class IBuildable;
class UBuildingCollection;
class UResourceCollection;

struct FBuildingAssetInfo;
struct FResourceAssetInfo;
struct FResourceValue;

/**
 * 
 */
UCLASS()
class GENERICBUILDER_API AGenericBuilderGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	typedef TArray<FResourceValue> FResourceData;

public:

	// Get the size of the minimum step adjusted to the landscape Grid.
	inline const float GetStepSize() const { return GridUnitSize / MaxGridSubdivision; }

	// Get the size of the minimum step adjusted to the landscape Grid.
	inline const float GetGridUnitSize() const { return GridUnitSize; }

	// Adds the given target building to the game building list
	void RegisterNewBuilding(IBuildable* Target);

	// Removes the given building from the game building list.
	void RemoveBuilding(IBuildable* Target);

	// Gets the index of the given player in the player list. Returns -1 if not found
	inline const int GetPlayerIndex(ABuilderPlayerPawn* Player) const;

	// Gets the available buildings list for this game mode.
	UFUNCTION(BlueprintCallable)
	const TArray<FBuildingAssetInfo>& GetAvailableBuildings();

	// Gets the available resources list for this game mode.
	UFUNCTION(BlueprintCallable)
	const TArray<FResourceAssetInfo>& GetAvailableResources();

	// Gets Default Resource Collection set in this game mode.
	// Resturns nullptr if not set
	inline UResourceCollection* GetDefaultResourceCollection();

private:

	// Collection of buildings used in this mode
	UPROPERTY(EditAnywhere, Category = "Builder Settings")
	TSubclassOf<UBuildingCollection> BuildingCollection;

	// Collection of resources used in this mode
	UPROPERTY(EditAnywhere, Category = "Builder Settings")
	TSubclassOf<UResourceCollection> ResourceCollection;

	// Overall Size of the grid in lands, this grid size is multiplied by the BuildingXYExtent
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Builder Settings", meta = (AllowPrivateAccess = "true"))
	float GridUnitSize = 50.0f;

	// The max subdivision of units to discretelly move buildings.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Builder Settings", meta = (AllowPrivateAccess = "true"))
	float MaxGridSubdivision = 4.0f;

	// Current players in this game.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Builder Settings", meta = (AllowPrivateAccess = "true"))
	TMap<ABuilderPlayerPawn*, int> PlayerList;

	// All buildings existing in the current game.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Builder Settings", meta = (AllowPrivateAccess = "true"))
	TArray<IBuildable*> BuildingList;

};

