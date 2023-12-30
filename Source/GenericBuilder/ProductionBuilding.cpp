// Author: Danilo Brandolin


#include "ProductionBuilding.h"


AProductionBuilding::AProductionBuilding()
{
	BuildableType = TEnumAsByte<EGB_BuildableType>(EGB_BuildableType::Production);
}

// Gets the current output stock value.
inline const FResourceValue& AProductionBuilding::GetOutputStock() const
{
	return OutputStock;
}
// ---------------------------------------------------------------

// Gets the current input stock values array.
inline const TArray<FResourceValue>& AProductionBuilding::GetInputStock() const
{
	return InputStock;
}
// ---------------------------------------------------------------

// Tries to insert all of the given elements or the max amount that fits in the available space.
// Returns the remaining resources after insertion.
inline const int AProductionBuilding::InsertInput(const FResourceValue Resource)
{

	FResourceValue* StockResource = InputStock.FindByKey(Resource.ID);

	if (StockResource != nullptr)
	{
		const int AvailableSpace = FMath::Clamp(MaxInputStock - StockResource->Value, 0, MaxInputStock);
		const int InsertedAmount = ((AvailableSpace - Resource.Value) <= 0) ? AvailableSpace : Resource.Value;

		StockResource->Value += InsertedAmount;

		return InsertedAmount;
	}

	return 0;

}
// ---------------------------------------------------------------

// Yield the given max Amount of output resource. 
// Returns the amount of resource that could be yielded.
inline const int AProductionBuilding::YieldOutput(const int Amount)
{
	const int YieldedAmount = ((OutputStock.Value - Amount) < 0) ? OutputStock.Value : Amount;

	OutputStock.Value -= YieldedAmount;

	return YieldedAmount;

}
// ---------------------------------------------------------------

// Refresh the current stock based on this building variables.
// Call this every time unit.
void AProductionBuilding::RefreshStock()
{
	// If stock is full skip.
	if (OutputStock.Value >= MaxOutputStock)
	{
		return;
	}

	if (ConsumeInput())
	{
		OutputStock.Value += CalculateDeltaProduction();
		UE_LOG(LogTemp, Display, TEXT("stock: %f"), OutputStock.Value);
	}
}
// ---------------------------------------------------------------

// Needed updates to property data when related properties are changed in the editor.
void AProductionBuilding::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	// If InputPerUnit is edited in the Editor, then re-initialize input stock to fit the input IDs.
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AProductionBuilding, InputPerUnit))
	{
		InitializeInputStock();
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);

}
// ---------------------------------------------------------------

// Consume input for 1 unit of production if enough. Returns true if it was consumed successfully.
const bool AProductionBuilding::ConsumeInput()
{
	const int ResourceCount = InputPerUnit.Num();
	// If no resource required, return as consumed.
	if (ResourceCount < 1)
	{
		return true;
	}

	// Get remaining values.
	TArray<FResourceValue> NewValues;
	for (int i = 0; i < ResourceCount; ++i)
	{
		// Get consuming rate according to ProductivityRate
		const float Consumed = InputPerUnit[i].Value * CalculateDeltaProduction();
		NewValues.Push(FResourceValue(InputStock[i].ID, InputStock[i].Value - Consumed));
		// If some value is not enough, discard all.
		if (NewValues.Last().Value < 0)
		{
			return false;
		}
	}

	// Copy new values.
	InputStock = NewValues;

	return true;

}
// ---------------------------------------------------------------

// Internal function to initialize the InputStock Array according to any change in InputPerUnit
inline void AProductionBuilding::InitializeInputStock()
{
	// Just copy it and then reset values to 0.
	InputStock = InputPerUnit;
	for (FResourceValue& Element : InputStock)
	{
		Element.Value = 0;
	}
}
// ---------------------------------------------------------------

// Calculate delta production
inline const float AProductionBuilding::CalculateDeltaProduction()
{
	return (0.01f * ProductivityRate) / ProductionTime;
}
// ---------------------------------------------------------------