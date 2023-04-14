// Copyright Epic Games, Inc. All Rights Reserved.


#include "ShooterGameModeBase.h"

AShooterGameModeBase::AShooterGameModeBase(){

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.0f,
			FColor::Yellow,
			FString(TEXT("It works"))
		);
	}
        
}
