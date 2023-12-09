// Author: Danilo Brandolin

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerVault.generated.h"

class UResourceCollection;
struct FResourceRep;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GENERICBUILDER_API UPlayerVault : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerVault();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FResourceRep> Resources;

	inline void InitializeVault(const UResourceCollection& ResourceCollection);
};
