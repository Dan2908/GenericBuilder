// Author: Danilo Brandolin
// 11/13/2023

#include "BaseBuilding.h"
#include "Materials/MaterialInstanceDynamic.h"


// Sets default values
ABaseBuilding::ABaseBuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Building Mesh"));
	RootComponent = Mesh;

}
// ---------------------------------------------------------------

// Called every frame
void ABaseBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
// ---------------------------------------------------------------

// Sets the building appearance, this is used to create 
void ABaseBuilding::SetMaterialAspect(const FDynamicMaterialInfo MaterialInfo)
{
	for (UMaterialInstanceDynamic* MID : MIDs)
	{
		MID->SetVectorParameterValue(FName("Tint"), MaterialInfo.Color);
		MID->SetScalarParameterValue(FName("Opacity"), MaterialInfo.Opacity);
	}
}
// ---------------------------------------------------------------


// Called when the game starts or when spawned
void ABaseBuilding::BeginPlay()
{
	Super::BeginPlay();
	
	const int MaterialCount = Mesh->GetNumMaterials();
	for (int i = 0; i < MaterialCount; ++i)
	{
		MIDs.Push(Mesh->CreateAndSetMaterialInstanceDynamic(i));
	}
}
// ---------------------------------------------------------------


