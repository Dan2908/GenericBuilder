// Author: Danilo Brandolin
// 11/13/2023

#pragma once

#include "BaseBuilding.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GenericBuilderGameModeBase.h"

#include "BuilderComponent.generated.h"

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
	void AdjustBuildingPosition(FVector CloseLocation);

	// Holds a new building to handle its positioning.
	// Note: The object must be created outside.
	UFUNCTION(BlueprintCallable)
	const bool HoldBuilding(ABaseBuilding* Building);

	// Leave the current held building. (Just deletes the reference, not the building itself).
	// Returns the left building pointer if any.
	UFUNCTION(BlueprintCallable)
	ABaseBuilding* LeaveBuilding();

	// Make the checkings and returns if the building can be built in the pointed world location.
	UFUNCTION(BlueprintCallable)
	const bool GetCanBuildHere();

	// Sets the building aspect according to the correct placing in the world
	UFUNCTION(BlueprintCallable)
	void SetBuildingAspect(const bool PlaceOK);
	
	// Call this to rotate the building (Yaw) if there is one currently being held.
	void RotateBuilding(const float DeltaYaw);

	// Call this to actually place the building in the world.
	// Note: This function does not check placement, check GetCanBuildHere() before call this.
	const bool ConfirmBuilding();

	// Returns true if any other building or obstacle is overlapping the desired building location.
	inline const bool IsPlaceObstructed() const;

	// Returns true if there are a held building.
	inline const bool IsHoldingBuilding() const { return HeldBuilding != nullptr; }


private:

	ABaseBuilding* HeldBuilding;

	AGenericBuilderGameModeBase* CurrentGameMode;

	float GridUnitSize;
	float StepSize;

	// Auxiliar box helper to calculate in world placement
	FBox PlacementBox;

	// Aspect when is the placement is OK
	UPROPERTY(VisibleAnywhere, Category = "Dynamic Material Presets")
	FDynamicMaterialInfo GoodLocationAppearance = FDynamicMaterialInfo(FLinearColor::Green, 0.5f);
	// Aspect when is the placement is not OK
	UPROPERTY(VisibleAnywhere, Category = "Dynamic Material Presets")
	FDynamicMaterialInfo BadLocationAppearance = FDynamicMaterialInfo(FLinearColor::Red, 0.5f);
	// Default Aspect
	UPROPERTY(VisibleAnywhere, Category = "Dynamic Material Presets")
	FDynamicMaterialInfo DefaultAppearance = FDynamicMaterialInfo(FLinearColor::White, 1.0f);

	// Max distance difference between corners to consider buildable terrain
	UPROPERTY(EditAnywhere, Category = "Building Rules")
	float MaxCornerDifference = 200.0f;

	inline AGenericBuilderGameModeBase* GetGameMode() { return Cast<AGenericBuilderGameModeBase>(GetWorld()->GetAuthGameMode()); }

	// Get Location adjusted to map grid.
	inline void GetRoundedLocation(FVector& WorldLocation);
	
};


	/*
	struct CornersOffsets
	{
		CornersOffsets(const FVector Location, const ABaseBuilding& Building, const float UnitSize);

		FVector CornerA;
		FVector CornerB;
		FVector CornerC;
		FVector CornerD;

		// Trace raw corners to the ground, to get acturl Location of the corners in the world
		const bool TraceCornersToGround(UWorld* World);
		// Returns true if none of the corners differs more then "Tolerance" 
		inline const bool CheckCornersHeightsV1(const float Tolerance = 25.0f)
		{
			const float Avg = GetHeightAverage();

			return (Avg - FMath::Abs(CornerA.Z) < Tolerance)
				&& (Avg - FMath::Abs(CornerB.Z) < Tolerance)
				&& (Avg - FMath::Abs(CornerC.Z) < Tolerance)
				&& (Avg - FMath::Abs(CornerD.Z) < Tolerance);
		}

		const FRotator GetCorrectedRotation(const FRotator CurrentRotation = FRotator())
		{
			// Roll(X) and Pitch(Y)
			const FVector DiagonalA = (CornerA - CornerD).GetSafeNormal();
			const FVector DiagonalB = (CornerB - CornerC).GetSafeNormal();
			FPlane Plane(DiagonalA, CornerC);
		
			UE_LOG(LogTemp, Display, TEXT("DiagonalA.Rotation() %s"), *DiagonalA.Rotation().ToString());
			UE_LOG(LogTemp, Display, TEXT("DiagonalB.Rotation() %s"), *DiagonalB.Rotation().ToString());
			UE_LOG(LogTemp, Display, TEXT("DiagonalA.Rotation().GetNormalized() %s"), *DiagonalA.Rotation().GetNormalized().ToString());
			UE_LOG(LogTemp, Display, TEXT("DiagonalB.Rotation().GetNormalized() %s"), *DiagonalB.Rotation().GetNormalized().ToString());

			
			FRotator PlaneRotation = Plane.Rotation();

			return PlaneRotation;
		}

	private:
		//	Calculate raw corners around a center point. These need to be traced to the ground to find the Z location.
		//  To set the corners to the world, use TraceCornersToGround(World).
		void CalculateCorners(const FVector& Center, const ABaseBuilding& Building, const float UnitSize);
		// Get The average height of the corners
		inline const float GetHeightAverage() 
		{ 
			return FMath::Abs((CornerA.Z + CornerB.Z + CornerC.Z + CornerD.Z) / 4); 
		}
	};
	*/