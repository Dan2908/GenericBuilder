// Author: Danilo Brandolin


#include "Buildable.h"

#include "BuilderPlayerState.h"
#include "GenericBuilderGameModeBase.h"

const FPreviewMaterialSettings GoodLocationAppearance = FPreviewMaterialSettings(FLinearColor::Green, 0.5f);
const FPreviewMaterialSettings BadLocationAppearance = FPreviewMaterialSettings(FLinearColor::Red, 0.5f);
const FPreviewMaterialSettings DefaultAppearance = FPreviewMaterialSettings(FLinearColor::White, 1.0f);

// Get the current available buildings from the current game mode collection
const TArray<FBuildingAssetInfo>& IBuildable::GetAvailableBuildings()
{
	if (AGenericBuilderGameModeBase* GM = FetchBuilderGM())
	{
		return GM->GetAvailableBuildings();
	}

	return TArray<FBuildingAssetInfo>();

}
// ---------------------------------------------------------------

// Tries to get builder game mode from the current object's world. Returns null if not found.
inline AGenericBuilderGameModeBase* IBuildable::FetchBuilderGM()
{
	if (UObject* ObjectPtr = Cast<UObject>(this))
	{
		return Cast<AGenericBuilderGameModeBase>(ObjectPtr->GetWorld()->GetAuthGameMode());
	}

	return nullptr;
}
// ---------------------------------------------------------------

// Generate Dynamic Material Instances from existing mesh materials. Returns true if all materials could be created.
const bool IBuildable::GeneratePreviewMaterials(UStaticMeshComponent* Mesh)
{
	if (!Mesh)
	{
		return false;
	}

	const int MaterialCount = Mesh->GetNumMaterials();
	for (int i = 0; i < MaterialCount; ++i)
	{
		DMIs.Push(Mesh->CreateAndSetMaterialInstanceDynamic(i));
	}

	return MaterialCount == DMIs.Num();
}
// ---------------------------------------------------------------

// Sets the building appearance, this is used to mainly to create previews and tweak colors. 
inline void IBuildable::SetMaterialAspect(const FPreviewMaterialSettings MaterialInfo)
{
	for (UMaterialInstanceDynamic* MID : DMIs)
	{
		MID->SetVectorParameterValue(FName("Tint"), MaterialInfo.Color);
		MID->SetScalarParameterValue(FName("Opacity"), MaterialInfo.Opacity);
	}
}
// ---------------------------------------------------------------

// Check if can be built and use DMIs to set preview aspect
inline void IBuildable::CheckAndSetAspect(ABuilderPlayerState* PlayerState)
{
	bool CanBuild = true;

	if (!CanAfford(PlayerState->PlayerResources))
	{
		UE_LOG(LogTemp, Warning, TEXT("Can afford this building!"));
		CanBuild = false;
	}

	if (!IsObstructed())
	{
		UE_LOG(LogTemp, Warning, TEXT("Building obstructed!"));
		CanBuild = false;
	}
	
	if (!IsLandRight())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bad location!"));
		CanBuild = false;
	}

	SetMaterialAspect(CanBuild ? GoodLocationAppearance : BadLocationAppearance);

}
// ---------------------------------------------------------------

