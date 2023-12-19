// Author: Danilo Brandolin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Game/BuildingAssetInfo.h"

#include "BuilderPlayerState.generated.h"

struct FResourceVault;
class ABaseBuilding;
class AProductionBuilding;

/**
 * 
 */
UCLASS()
class GENERICBUILDER_API ABuilderPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	
	// Container with the resources of the player.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Belongings", meta = (AllowPrivateAccess = "true"))
	FResourceVault PlayerResources;
	// List of buildings constructed by this player.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Belongings")
	TArray<ABaseBuilding*> PlayerBuildings;
	// Checks if a payment can be done with the current PlayerResources. The OutCalculation is filled with the resources remaining.
	// Returns true if all resources can be paid, false if at least 1 is not enough.
	UFUNCTION()
	const bool Pay(const TArray<FResourceValue>& CostArray);
	// Register a buildingto  belong to this player
	UFUNCTION()
	void RegisterBuilding(ABaseBuilding* NewBuilding, const EGB_BuildingTypes BuildingType);
	// Unregister a building that belongs to this player. Returns false if OurBuilding is not found in this player's list.
	UFUNCTION()
	const bool UnregisterBuilding(ABaseBuilding* OurBuilding);
	// Calls UpdateProductionTask() when accummulated ProductionUpdateCounter reaches ProductionUpdateTime.
	void UpdateProductionCount(float DeltaTime);
	// Updates the production stock for every Production building of this player.
	void UpdateProductionTask();
	// Retrieves Production task
	inline UE::Tasks::FTask& GetProductionTask();

private:

	// Time in seconds to update the production count.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float ProductionUpdateTime = 1.0f;
	// Used to accumulate timer in order to launch the update task
	float ProductionUpdateCounter = 0.0f;
	// Parallel task to perform production updates async.
	UE::Tasks::FTask ProductionTask = {};
	// Auxiliar list of production buildings constructed by this player.
	TArray<AProductionBuilding*> ProductionBuildings;
};
