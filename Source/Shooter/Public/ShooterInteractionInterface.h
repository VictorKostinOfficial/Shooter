#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ShooterInteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UShooterInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class SHOOTER_API IShooterInteractionInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(APawn* InstigatorPawn);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FText GetInteractText(APawn* InstigatorPawn);
};



