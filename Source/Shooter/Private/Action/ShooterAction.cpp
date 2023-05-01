// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/ShooterAction.h"

// void UShooterAction::Initialize(UShooterActionComponent* NewActionComp)
// {
//     ActionComp = NewActionComp;
// }

void UShooterAction::StartAction_Implementation(AActor *Instigator)
{
    UE_LOG(LogTemp, Log, TEXT("Started: %s"), *GetNameSafe(this));
}

void UShooterAction::StopAction_Implementation(AActor* Instigator)
{
    UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
}

UWorld *UShooterAction::GetWorld() const
{
	// Outer is set when creating action via NewObject<T>
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp)
	{
		return Comp->GetWorld();
	}

	return nullptr;
}
