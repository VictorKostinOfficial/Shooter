// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/ShooterAction.h"
#include "Action/ShooterActionComponent.h"
#include "Net/UnrealNetwork.h"


void UShooterAction::Initialize(UShooterActionComponent* NewActionComp)
{
	CoolDown = 10.f;
    ActionComp = NewActionComp;
}

bool UShooterAction::CanStart_Implementation(AActor *Instigator)
{
	if(IsRunning()){
		return false;
	}

	UShooterActionComponent* Comp = GetOwningComponent();

	if(Comp->ActiveGameplayTags.HasAny(BlockedTags)){
		return false;
	}

	return true;
}

void UShooterAction::StartAction_Implementation(AActor *Instigator)
{
    UE_LOG(LogTemp, Log, TEXT("Started: %s"), *GetNameSafe(this));
	
	UShooterActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;

	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "CoolDownDuration_Elapsed");
	GetWorld()->GetTimerManager().SetTimer(CoolDownDurationHandle, Delegate, CoolDown, false);
}

void UShooterAction::StopAction_Implementation(AActor* Instigator)
{
    UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));

	ensureAlways(RepData.bIsRunning);

	UShooterActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);
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

UShooterActionComponent* UShooterAction::GetOwningComponent() const
{
	return Cast<UShooterActionComponent>(GetOuter());
}

bool UShooterAction::IsRunning() const
{
    return RepData.bIsRunning;
}

void UShooterAction::CoolDownDuration_Elapsed()
{
	RepData.bIsRunning = false;
}

void UShooterAction::OnRep_RepData()
{
	if(RepData.bIsRunning){
		StartAction(RepData.Instigator);
	}
	else{
		StopAction(RepData.Instigator);
	}

}

void UShooterAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UShooterAction, RepData);
	DOREPLIFETIME(UShooterAction, TimeStarted);
	DOREPLIFETIME(UShooterAction, ActionComp);
}
