// Author: Danilo Brandolin

#pragma once

#include "CoreMinimal.h"
#include "BaseBuilding.h"

#include "StorageBuilding.generated.h"

class ABuilderPlayerState;
struct FResourceValue;

/**
 * 
 */
UCLASS()
class GENERICBUILDER_API AStorageBuilding : public ABaseBuilding
{
	GENERATED_BODY()

public:

	AStorageBuilding();
	// Set the owning player state reference.
	UFUNCTION()
	void SetBuilderPlayerState(ABuilderPlayerState* PlayerState);

	// Adds the given resource amount to the owning player vault.
	UFUNCTION()
	void AddResource(const FResourceValue& Resource);

private:

	ABuilderPlayerState* BuilderPlayerState;
	
};
