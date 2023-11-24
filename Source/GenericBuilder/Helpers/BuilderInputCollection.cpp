// Author: Danilo Brandolin

#include "Helpers/BuilderInputCollection.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


// Start Up variables
void UBuilderInputCollection::StartInputObject(UInputComponent* PlayerInputComponent, AController* Controller)
{
	EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	//Add Input Mapping Context
	// just supporting 1 IMC, for more there must be Enable/Disable function independently.
	if (APlayerController* PlayerController = CastChecked<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}


}
// ---------------------------------------------------------------

