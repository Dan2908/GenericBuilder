// Author: Danilo Brandolin

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BuilderInputCollection.generated.h"


class UInputAction;

/**
 * Manages Input
 */
UCLASS(Blueprintable)
class GENERICBUILDER_API UBuilderInputCollection : public UObject
{
	GENERATED_BODY()

public:

	// Start Up variables
	void StartInputObject(UInputComponent* PlayerInputComponent, AController* Controller);

	// Set up action bind with triggerEvent "Triggered".
	template<typename T, class FuncType>
	void BindTriggerAction(T* TargetObject, UInputAction* InputAction, FuncType Function);

	/** DefaultMappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mapping")
	class UInputMappingContext* DefaultMappingContext;
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mapping")
	class UInputAction* MoveAction;
	/** Rotate Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mapping")
	class UInputAction* RotateAction;
	/** Zoom Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mapping")
	class UInputAction* ZoomAction;
	/** Rotate building Action when contructing */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mapping")
	class UInputAction* RotateHeldBuildingAction;
	/** Normally click press, when to trigger a building or a demolition */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mapping")
	class UInputAction* ConfirmAction;
	/** Cancel action, for example quit placin a building */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mapping")
	class UInputAction* EscapeAction;

private:

	UEnhancedInputComponent* EnhancedInputComponent;
	
};


template<typename T, class FuncType>
inline void UBuilderInputCollection::BindTriggerAction(T* TargetObject, UInputAction* InputAction, FuncType Function)
{
	EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, TargetObject, Function);
}
