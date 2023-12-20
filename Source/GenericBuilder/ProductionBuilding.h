// Author: Danilo Brandolin

#pragma once

#include "CoreMinimal.h"
#include "BaseBuilding.h"
#include "Definitions/Resources.h"
#include "Game/ResourceAssetInfo.h" 

#include "ProductionBuilding.generated.h"

/**
 * Base class for Production Buildings. Meaning this produce any kind of resource over time.
 */
UCLASS()
class GENERICBUILDER_API AProductionBuilding : public ABaseBuilding
{
	GENERATED_BODY()
	
public:

	AProductionBuilding();

	// Gets the current output stock value.
	inline const FResourceValue& GetOutputStock() const;
	// Gets the current input stock values array.
	inline const TArray<FResourceValue>& GetInputStock() const;

	// Tries to insert all of the given elements or the max amount that fits in the available space.
	// Returns the remaining resources after insertion.
	inline const int InsertInput(const FResourceValue Resource);

	// Yield the given max Amount of output resource. 
	// Returns the amount of resource that could be yielded.
	inline const int YieldOutput(const int Amount);

	// Refresh the current stock based on this building variables.
	// Call this every time unit.
	void RefreshStock();

protected:

	// Needed updates to property data when related properties are changed in the editor.
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

private:

	// Current stock of output resources.
	UPROPERTY(EditAnywhere, Category = "Production Setup", meta = (AllowPrivateAccess = "true"))
	FResourceValue OutputStock;
	// Current stock of input resources.
	UPROPERTY(EditAnywhere, Category = "Production Setup", meta = (AllowPrivateAccess = "true"))
	TArray<FResourceValue> InputStock;

	// Input resources needed to produce 1 unit of OutputResource
	UPROPERTY(EditAnywhere, Category = "Production Setup", meta = (AllowPrivateAccess = "true"))
	TArray<FResourceValue> InputPerUnit;

	// Percentage of productivity
	UPROPERTY(EditAnywhere, Category = "Production Setup", meta = (AllowPrivateAccess = "true"))
	float ProductivityRate = 100.0f;

	// Time that this building takes to produce 1 unit.
	UPROPERTY(EditAnywhere, Category = "Production Setup", meta = (AllowPrivateAccess = "true"))
	float ProductionTime = 4.0f;

	// Max stock capacity of output resources.
	UPROPERTY(EditAnywhere, Category = "Production Setup", meta = (AllowPrivateAccess = "true"))
	int MaxOutputStock = 5;

	// Max stock capacity of input resources.
	UPROPERTY(EditAnywhere, Category = "Production Setup", meta = (AllowPrivateAccess = "true"))
	int MaxInputStock = 2;

	// Consume input for 1 unit of production if enough. Returns true if it was consumed successfully.
	const bool ConsumeInput();

	// Internal function to initialize the InputStock Array according to any change in InputPerUnit
	inline void InitializeInputStock();

	// Calculate delta production
	inline const float CalculateDeltaProduction();
};
