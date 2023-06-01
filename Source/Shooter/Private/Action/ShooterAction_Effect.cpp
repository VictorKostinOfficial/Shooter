// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/ShooterAction_Effect.h"
#include "Action/ShooterActionComponent.h"
#include "GameFramework/GameStateBase.h"


UShooterAction_Effect::UShooterAction_Effect()
{
    bAutoStart = true;
}

void UShooterAction_Effect::StartAction_Implementation(AActor * Instigator)
{
    Super::StartAction_Implementation(Instigator);

    if (Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}

	if (Period > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}

}

void UShooterAction_Effect::StopAction_Implementation(AActor *Instigator)
{
    if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}

	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);

	UShooterActionComponent* Comp = GetOwningComponent();
	if (Comp && bAutoStart)
	{
		Comp->RemoveAction(this);
	}
}

float UShooterAction_Effect::GetTimeRemainig() const
{
    AGameStateBase* GS = GetWorld()->GetGameState<AGameStateBase>();
	if (GS)
	{
		float EndTime = TimeStarted + Duration;
		return EndTime - GS->GetServerWorldTimeSeconds();
	}

	return Duration;
}

void UShooterAction_Effect::ExecutePeriodicEffect_Implementation(AActor *Instigator)
{
	// Logic in derived class
}
