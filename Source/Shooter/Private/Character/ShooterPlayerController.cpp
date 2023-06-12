// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/ShooterPlayerController.h"

void AShooterPlayerController::SetPawn(APawn * InPawn)
{
    Super::SetPawn(InPawn);

    OnPawnChanged.Broadcast(InPawn);

}