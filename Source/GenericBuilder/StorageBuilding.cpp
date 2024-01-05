// Author: Danilo Brandolin

#include "StorageBuilding.h"

#include "BuilderPlayerState.h"
#include "Definitions/Resources.h"

// Constructor
AStorageBuilding::AStorageBuilding()
{
	BuildableType = TEnumAsByte<EGB_BuildableType>(EGB_BuildableType::Storage);
	 
}

// Set the owning player state reference.
void AStorageBuilding::SetBuilderPlayerState(ABuilderPlayerState* PlayerState)
{
	BuilderPlayerState = PlayerState;

}
// ---------------------------------------------------------------

// Adds the given resource amount to the owning player vault.
void AStorageBuilding::AddResource(const FResourceValue& Resource)
{
	BuilderPlayerState->PlayerResources.AddResource(Resource);
	
}
// ---------------------------------------------------------------

