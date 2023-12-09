// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "GenericBuilderGameModeBase.generated.h"

class ABuilderPlayerPawn;
class ABaseBuilding;
class UBuildingCollection;
class UResourceCollection;

struct FBuildingAssetInfo;
struct FResourceAssetInfo;
struct FResourceRep;


/**
 * 
 */
UCLASS()
class GENERICBUILDER_API AGenericBuilderGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	typedef TArray<FResourceRep> FResourceData;

public:

	// Get the size of the minimum step adjusted to the landscape Grid.
	inline const float GetStepSize() const { return GridUnitSize / MaxGridSubdivision; }

	// Get the size of the minimum step adjusted to the landscape Grid.
	inline const float GetGridUnitSize() const { return GridUnitSize; }

	void NewBuilding(ABaseBuilding* Target);

	void DestroyBuilding(ABaseBuilding* Target);

	inline const int GetPlayerIndex(ABuilderPlayerPawn* Player) const;

	UFUNCTION(BlueprintCallable)
	const TArray<FBuildingAssetInfo>& GetAvailableBuildings();

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Builder Settings", meta = (AllowPrivateAccess = "true"))
	float MaxGridSubdivision = 4.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Builder Settings", meta = (AllowPrivateAccess = "true"))
	TMap<ABuilderPlayerPawn*, int> PlayerList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Builder Settings", meta = (AllowPrivateAccess = "true"))
	TArray<ABaseBuilding*> BuildingList;

};

