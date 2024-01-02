// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/BuildingAssetInfo.h"
#include "UObject/NoExportTypes.h"

#include "BuildingCollection.generated.h"

class ABaseBuilding;
class UTexture2D;
class UResourceCollection;

/**
	This object is meant to hold a list of Building Asset Information that can be assigned to a level/gamemode.
	Building Asset Information contains general customizable information about a building, see FBuildingAssetInfo.
*/
UCLASS(Blueprintable)
class GENERICBUILDER_API UBuildingCollection : public UObject
{
	GENERATED_BODY()

public:

	// Constructor
	UBuildingCollection();

	// Gets const array of buildings in this collection.
	UFUNCTION(BlueprintCallable)
	const TArray<FBuildingAssetInfo>& GetBuildings() const;

	// Called when a property on this object has been modified externally.
	// @param PropertyThatChanged the property that was modified.
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

private:

	// Array Building Asset Information, containing ID, Thumbnails, references, etc.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collection", meta = (AllowPrivateAccess = "true"))
	TArray<FBuildingAssetInfo> Buildings;

};
