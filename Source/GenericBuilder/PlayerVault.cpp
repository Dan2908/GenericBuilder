// Author: Danilo Brandolin


#include "PlayerVault.h"

#include "Game/ResourceCollection.h"


// Sets default values for this component's properties
UPlayerVault::UPlayerVault()
	: Resources()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UPlayerVault::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UPlayerVault::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

inline void UPlayerVault::InitializeVault(const UResourceCollection& ResourceCollection)
{
	// ResourceCollection.FillDefaultResourceData(Resources);
}

