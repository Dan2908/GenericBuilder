// Author: Danilo Brandolin

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Game/ResourceCollection.h"

#include "PlayerVault.generated.h"

class UResourceCollection;
struct FResourceRep;

UCLASS()
class GENERICBUILDER_API UPlayerVault : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerVault();

	// Wrapper function to call this vault's PreviewCost(), returning true if there are enough
	const bool PreviewCost(const TArray<FResourceRep>& Cost, TArray<FResourceRep>& Remaining) const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
	FResourceVault Resources;

};
