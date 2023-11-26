// Author: Danilo Brandolin
// 11/13/2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Definitions/BuildingTypes.h"
#include "Definitions/Buildings.h"

#include "BaseBuilding.generated.h"

// --------------------------
// Struct FDynamicMaterialInfo
// --------------------------

USTRUCT(BlueprintType)
struct FDynamicMaterialInfo
{
	GENERATED_BODY()

	FDynamicMaterialInfo()
		: Color(FLinearColor::White)
		, Opacity(1)
	{}

	FDynamicMaterialInfo(const FLinearColor Color, const float Opacity)
		: Color(Color)
		, Opacity(Opacity)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dynamic Material Parameters")
	FLinearColor Color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dynamic Material Parameters")
	float Opacity;

};


// ---------------------------------
// ------ CLASS ABaseBuilding ------
// ---------------------------------

UCLASS()
class GENERICBUILDER_API ABaseBuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseBuilding();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Sets the building appearance, this is used to create the "placing" aspect
	void SetMaterialAspect(const FDynamicMaterialInfo MaterialInfo);
	// Get Buildiung extents
	inline const FVector2D& GetXYExtents() { return Extents; }

private:
	// Base mesh
	UPROPERTY(VisibleAnywhere, Category = "Building Adjustments")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Building Attributes")
	FVector2D Extents;

	// Material Instance Dynamic
	TArray<class UMaterialInstanceDynamic*> MIDs;

};
