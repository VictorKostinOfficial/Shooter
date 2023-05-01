// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/ShooterActionComponent.h"
#include "Action/ShooterAction.h"
#include "Net/UnrealNetwork.h"

#define D(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, x);}

UShooterActionComponent::UShooterActionComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

}

void UShooterActionComponent::BeginPlay()
{

	Super::BeginPlay();

	for (TSubclassOf<UShooterAction> ActionClass : DefaultActions)
	{
		AddAction(ActionClass);
	}
	
}

void UShooterActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UShooterActionComponent::AddAction(TSubclassOf<UShooterAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UShooterAction* NewAction = NewObject<UShooterAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
}

bool UShooterActionComponent::StartActionByName(AActor *Instigator, FName ActionName)
{
	for (UShooterAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			D(TEXT(" " + ActionName.ToString() + " "));
			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool UShooterActionComponent::StopActionByName(AActor *Instigator, FName ActionName)
{
	for (UShooterAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StopAction(Instigator);
			return true;
		}
	}

    return false;
}

// void UShooterActionComponent::ServerStartAction(AActor *Instigator, FName ActionName)
// {
// 	StartActionByName(Instigator, ActionName);
// }

// void UShooterActionComponent::ServerStopAction(AActor *Instigator, FName ActionName)
// {
// 	StopActionByName(Instigator, ActionName);
// }

void UShooterActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UShooterActionComponent, Actions);
}