// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/ShooterActionComponent.h"
#include "Action/ShooterAction.h"
#include "Net/UnrealNetwork.h"

#define D(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, x);}

UShooterActionComponent::UShooterActionComponent()
{
	
	SetIsReplicatedByDefault(true);

}

void UShooterActionComponent::BeginPlay()
{

	Super::BeginPlay();

	for (TSubclassOf<UShooterAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
	
}

void UShooterActionComponent::AddAction(AActor* Instigator, TSubclassOf<UShooterAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UShooterAction* NewAction = NewObject<UShooterAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);

		if(NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void UShooterActionComponent::RemoveAction(UShooterAction* ActionClass)
{
	if(!ensure(ActionClass && !ActionClass->IsRunning())){
		return;
	}

	Actions.Remove(ActionClass);
}

void UShooterActionComponent::StartActionByName(AActor *Instigator, FName ActionName)
{
	ServerStartAction(Instigator, ActionName);
}

void UShooterActionComponent::StopActionByName(AActor *Instigator, FName ActionName)
{
	ServerStopAction(Instigator, ActionName);
}

void UShooterActionComponent::ServerStartAction_Implementation(AActor *Instigator, FName ActionName)
{
	for (UShooterAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if(!Action->CanStart(Instigator)){
				return;
			}

			D(TEXT(" " + ActionName.ToString() + " "));
			Action->StartAction(Instigator);
			return;
		}
	}
	// MulticastStartAction(Instigator, ActionName);
}

void UShooterActionComponent::ServerStopAction_Implementation(AActor *Instigator, FName ActionName)
{
	for (UShooterAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if(Action->IsRunning()){
				Action->StopAction(Instigator);
				return;
			}
		}
	}
	// MulticastStopAction(Instigator, ActionName);
}

void UShooterActionComponent::MulticastStartAction_Implementation(AActor *Instigator, FName ActionName)
{
}

void UShooterActionComponent::MulticastStopAction_Implementation(AActor *Instigator, FName ActionName)
{
}

void UShooterActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UShooterActionComponent, Actions);
}